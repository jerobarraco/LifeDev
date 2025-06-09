// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroMan.h"

#include "Kismet/GameplayStatics.h"

#include "JUtils/Misc/JUtilsMisc.h"

#include "LifeDev/Core/Consts/ConstSettings.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

#include "IntroUI.h"
#include "LoadScr.h"

AIntroMan::AIntroMan():Super() {
	static ConstructorHelpers::FClassFinder<UIntroUI>
		CUI(TEXT("/Game/LifeDev/Intro/UI/IntroUI_W"));
	UIClass = CUI.Class;

	if (IsRunningCookCommandlet()) {
		MusicNew.LoadSynchronous(); // ensure it gets packaged
		Music.LoadSynchronous();
	}
}

void AIntroMan::AddUI() {
	if (UNLIKELY(!UIClass || !UIClass.Get())) return;

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UI = CreateWidget<UIntroUI>(World, UIClass.Get());
	if (UNLIKELY(!IsValid(UI))) return;
	
	UI->AddToViewport();
	UI->OnDone.AddDynamic(this, &AIntroMan::Done);
	UI->OnSlotsDone.AddDynamic(this, &AIntroMan::SlotsDone);

	UJUtilsMisc::ShowUI(this, true, UI);
}

void AIntroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	const UWorld* const World = GetWorld();
	ULSettings* const Settings = ULSettings::Instance(World);
	if (UNLIKELY(!Settings)) return;
	if (UNLIKELY(!Settings->Save)) {
		Settings->NewGame();
		if (UNLIKELY(!Settings->Save)) {
			UE_LOG(LogTemp, Warning, TEXT("%hs: Current Save is not set! Can't work under these conditions!."),
				__func__);
			static const FText TheEnd(
			NSLOCTEXT("Intro", "Can't save",
				"Savegame was erroneous and a new one could not be created.\n"
				"No idea what went wrong, check the logs if any.\n"
				"Sorry."));
			UI->ShowMsg(TheEnd);
			return;
		}
	}
	
	// savestate actually saves past the last chapter.
	const EFeat ChapterFeat = Settings->CurrentChapterFeat();

	if (UNLIKELY(ChapterFeat >= EFeat::C_DONE)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs: Current save is beyond the max chapter."), __func__);
		static const FText TheEnd(
			NSLOCTEXT("Intro", "MaxChapterReached",
				"This save is at the end.\n"
				"Maybe in a distant future i'll add more.\n"
				"But for now you can't go further."));
		UI->ShowMsg(TheEnd);
		return;
	}
	/// all ok, continue.
	
	UJUtilsMisc::ShowUI(this, false);
	

	// this is a patch to ensure the settings are respected when going to the game.
	// as well as the foxify value.
	// the false is important since the inventory does not work on the intro.
	// reading subsystems is important since the settings and flags could change.
	Settings->Save->ReadSubsystems(this, false);
	Settings->OnSaving.AddUniqueDynamic(this, &AIntroMan::Saving);
	Settings->SaveGame();
}

void AIntroMan::Saving(const bool IsSaving) {
	if (IsSaving) return;

	// once it finished saving, load the world

	ULoadScr* const Load = ULoadScr::Instance(this);
	if (Load)
		Load->Show();

	// this is actually not needed since the game mode is set on the world settings
	// but if we were to need it here it is. we will need to add to the game mode aliases on the map&modes settings, under advanced
	// FString Options = "Game="+ NextLevelMode;
	// UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true, Options);
	const FName& NextLevel = ULSysSettings::GetGameLvl();
	UGameplayStatics::OpenLevel(GetWorld(), NextLevel, true);
}

void AIntroMan::BeginPlay() {
	Super::BeginPlay();
	AddUI();
}

void AIntroMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(UI)) {
		UI->OnDone.RemoveAll(this);
		UI->OnSlotsDone.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AIntroMan::SlotsDone(const bool HasDoneSave) {
	ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (UNLIKELY(!Man)) return;

	// the whole issue was i was not initializing this instance on intro game mode. :')
	USoundBase* const M = HasDoneSave ? MusicNew.LoadSynchronous() : Music.LoadSynchronous();
	Man->PlayMusic(M, true);
}
