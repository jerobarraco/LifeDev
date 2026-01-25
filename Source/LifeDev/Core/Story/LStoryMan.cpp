// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LStoryMan.h"

#include "Diags/Diags.h"
#include "Diags/DiagTypes.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Sentry.h"
#include "LifeDev/Core/Settings/LFeatsMan.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Env/Ghost/GhostPool.h"

#include "Story/StoryUI.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "Story/Story.h"

DEFINE_LOG_CATEGORY_STATIC(LogLStoryMan, Log, Log)

ALStoryMan* ALStoryMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld(); 
	if (UNLIKELY(!IsValid(World))) return nullptr;
	
	const ALGGameMode* const GM = Cast<ALGGameMode>(World->GetAuthGameMode());
	return LIKELY(GM) ?
		GM->StoryMan.Get() :
		Cast<ALStoryMan>(UGameplayStatics::GetActorOfClass(O, ALStoryMan::StaticClass()));
}

ALStoryMan::ALStoryMan():Super() {
	static ConstructorHelpers::FClassFinder<UStoryUI>
		CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_LStoryUI"));
	UIClass = LIKELY(CUI.Succeeded()) ? CUI.Class.Get() : UStoryUI::StaticClass(); // .Get is required for compilation
	// this contains a save icon animation that will only be seen during the transition between chapters
	// but since i've intentionally only save at that point, it works.
}

void ALStoryMan::BeginPlay() {
	Super::BeginPlay();
	Settings = ULSettings::Instance(this);
	UE_CLOG(UNLIKELY(!Settings), LogLStoryMan, Warning, TEXT("%hs Could not get the settings subsystem."), __func__);

	Diags = UDiags::Instance(this);
	UE_CLOG(UNLIKELY(!Diags), LogLStoryMan, Warning, TEXT("%hs Could not get the diags subsystem."), __func__);
}

void ALStoryMan::Init_Implementation() {
	Super::Init_Implementation();
	
	const ALGGameMode* const GM = ALGGameMode::Instance(this);
	UE_CLOG(UNLIKELY(!GM), LogLStoryMan, Warning, TEXT("%hs Could not get the game mode."), __func__);

	const ALFeatsMan* const Feats = ALFeatsMan::Instance(this);
	Ghosts = LIKELY(Feats) ? Feats->Ghosts : nullptr;
	UE_CLOG(UNLIKELY(!Ghosts), LogLStoryMan, Warning, TEXT("%hs Could not get the ghosts."), __func__);

	MusicMan = LIKELY(GM) ? GM->MusicMan : nullptr; // should i move the music man to the feats?
	UE_CLOG(UNLIKELY(!MusicMan), LogLStoryMan, Warning, TEXT("%hs Could not get the music manager."), __func__);
	
	if (LIKELY(Story)) Story->OnSeqStop.AddUniqueDynamic(this, &ALStoryMan::ChapStartNext);
}

void ALStoryMan::DeInit_Implementation() {
	if (LIKELY(Story)) Story->OnSeqStop.RemoveAll(this);

	// these could have been loaded from a json
	Chapter.Dialogs = nullptr;
	Chapter.Groups = nullptr;

	Diags = nullptr;
	Settings = nullptr;
	MusicMan = nullptr;
	Super::DeInit_Implementation();
}

void ALStoryMan::ChapStartEnd() const {
	// done this way to have also transitions. // notice short-circuit
	const bool Started = LIKELY(Story) && Story->Start(StepEndName);
	if (LIKELY(Started)) return;
	
	UE_LOG(LogLStoryMan, Warning, TEXT("%hs Could not start End step. Verify the name is correct and is added to the level! Skip."), __func__);
	// this is just a safety net because i do not like soft-locks.
	UGameplayStatics::OpenLevel(GetWorld(), FName("Outro_L"), true);
}

void ALStoryMan::ChapStartNext() {
	if (UNLIKELY(!IsValid(Settings->Save))) {
		UE_LOG(LogLStoryMan, Warning, TEXT("%hs: Savegame is null. can't progress."), __func__);
		return;
	}

	// Chapter done. go to the next one. important before savegame. read note on chapterId
	Settings->Save->ChapterID++;

	// this won't trigger at the first start. read note on chapterId.
	Settings->SaveGame();
	
	// can't remember if this happens during the fade out. but i'm confident i would have coded it that way.
	// clean the ghosts
	if (LIKELY(Ghosts)) Ghosts->Kill(true);

	ChapStart();
}

void ALStoryMan::ChapStart() {
	const int32 ChapterId = Settings->CurrentChapter();
	const EFeat& ChapFeat = Settings->CurrentChapterFeat();

	UE_LOG(LogLStoryMan, Log, TEXT("%hs Attempting to start chapter id=%i feat=%s"),
		__func__, ChapterId, *UEnum::GetValueAsString(ChapFeat));

	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (UNLIKELY(!IsValid(Instance) | !IsValid(Story))) {
		// Should this be here?
		UE_LOG(LogLStoryMan, Warning, TEXT("%hs No game instance or story or story manager or game mode. Can't proceed."),
			__func__);
		return;
	}

	const USentry* const Sentry = USentry::Instance(this);
	const FString& ChapFeatS = UEnum::GetValueAsString(ChapFeat);
	if (LIKELY(Sentry)) {
		const TMap<FName, FString> Data = {{"ChapId",FString::FromInt(ChapterId)},
			{"Feat", ChapFeatS}};
		Sentry->AddHint(__func__, Data);
	}

	// stop here to avoid getting the engine stuck trying to load chapters
	if (UNLIKELY(ChapFeat >= EFeat::C_DONE)) {
		UE_LOG(LogLStoryMan, Warning, TEXT("%hs Went beyond available chapters. Stopping dry. id=%i."),
			__func__, ChapterId);
		ChapStartEnd();
		return;
	}

	// skip chapter if not enabled or just started
	if (UNLIKELY((ChapFeat == EFeat::NONE) | !Settings->GetFeat(ChapFeat))) {
		UE_LOG(LogLStoryMan, Warning, TEXT("%hs Skipping chapter. Not in game Feats. id=%i."),
			__func__, ChapterId);
		ChapStartNext(); // note this is recursive but there ain't that many chapters
		return;
	}

	/// load new one
	if (UNLIKELY(!ChapLoad())) {
		UE_LOG(LogLStoryMan, Warning, TEXT("%hs Chapter didn't load. Won't start any sequence."), __func__);
		if (LIKELY(Sentry)) Sentry->AddMsg("Failed to load chapter", ESentryLevel::Error);
		return;
	}

	// the story manager will make the gm disable/enable the input
	// Start the sequence.
	Story->StartSequence(Chapter.Steps); // maybe move here
	MusicMan->SetEnviron(true); // maybe move here
}

bool ALStoryMan::ChapLoad() {
	UE_LOG(LogLStoryMan, Log, TEXT("%hs"), __func__);

	if (UNLIKELY(!IsValid(Diags))) {
		UE_LOG(LogLStoryMan, Error, TEXT("%hs: Invalid diags subsystem."), __func__);
		USentry::SAddMsg(this, "Invalid Diag subsystem. Stop", ESentryLevel::Error);
		return false;
	}

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	const UDataTable* const DT_Chaps = SysSettings->Chapters.LoadSynchronous();
	if (UNLIKELY(!IsValid(DT_Chaps))) {
		UE_LOG(LogLStoryMan, Error, TEXT("%hs: Chapter Datatable is not properly set in the settings."), __func__);
		USentry::SAddMsg(this, "Could not obtain the chapter datatable from settings. Stop", ESentryLevel::Error);
		return false;
	}

	/// unload old chapter
	if (Chapter.Dialogs.IsValid())
		Diags->DTDiagRem(Chapter.Dialogs.Get());
	if (Chapter.Groups.IsValid())
		Diags->DTGroupRem(Chapter.Groups.Get());
	
	// DT_Chaps = UJUtilsMisc::LoadJSONTable(FPaths::ProjectConfigDir(), "test",
		// FLChapter::StaticStruct(), this); // cant find the symbol
	
	// load a chapter based on the rowname. which is just an int to string of the chapter id.
	// todo find a betterest way
	const FName ChapName = *FString::FromInt(Settings->CurrentChapter());
	FLChapter* const pChap = DT_Chaps->FindRow<FLChapter>(ChapName, TEXT(""));
	if (UNLIKELY(!pChap)) {
		UE_LOG(LogLStoryMan, Warning, TEXT("Can't get the chapter from datatable. Row=%s."), *ChapName.ToString());
		return false;
	}

	Chapter = *pChap; // Make a copy
	// set them on the dialog subsystem

	// probably need to load before, since i use getasset name below
	Chapter.Dialogs.LoadSynchronous();
	Chapter.Groups.LoadSynchronous();
	
	if (Settings && Settings->GetFeat(EFeat::G_DATA_EXT)) {
		static const FString& Base = FPaths::Combine(FPaths::ProjectConfigDir(), "L10N");
		TArray<FString> Problems;
		// wait, this is using dialogs and groups here
		UDataTable* const DiagExt = UJUtilsMisc::LoadJSONTable(Base,
			Chapter.Dialogs.GetAssetName(), FDiag::StaticStruct(), Problems, this);
		UDataTable* const GroupsExt = UJUtilsMisc::LoadJSONTable(Base,
			Chapter.Groups.GetAssetName(), FDiagGroup::StaticStruct(), Problems, this);
		Chapter.Dialogs = DiagExt; // best way. so it can be unloaded too.
		Chapter.Groups = GroupsExt; // best way. so it can be unloaded too.
	}

	Diags->DTDiagAdd(Chapter.Dialogs.Get());
	Diags->DTGroupAdd(Chapter.Groups.Get());

	return true;
}