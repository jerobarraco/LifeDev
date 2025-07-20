// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LStoryMan.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
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

	GM = ALGGameMode::Instance(this);
	UE_CLOG(UNLIKELY(!GM), LogLStoryMan, Warning, TEXT("%hs Could not get the game mode."), __func__);
}

void ALStoryMan::Init_Implementation() {
	Super::Init_Implementation();
	Ghosts = LIKELY(GM) ? GM->Ghosts : nullptr;
	UE_CLOG(UNLIKELY(!GM), LogLStoryMan, Warning, TEXT("%hs Could not get the ghosts."), __func__);

	if (LIKELY(Story)) Story->OnSeqStop.AddUniqueDynamic(this, &ALStoryMan::ChapStartNext);
}

void ALStoryMan::DeInit_Implementation() {
	if (LIKELY(Story)) Story->OnSeqStop.RemoveAll(this);

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

	if (LIKELY(GM)) GM->ChapStart();
}

