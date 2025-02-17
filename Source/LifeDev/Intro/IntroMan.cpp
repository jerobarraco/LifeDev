// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroMan.h"

#include "Kismet/GameplayStatics.h"

#include "JUtils/Misc/JUtilsMisc.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "IntroUI.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"

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

	UJUtilsMisc::ShowUI(this, true, UI, false);
}

void AIntroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	const UWorld* const World = GetWorld();

	const ULSettings* const Settings = ULSettings::Instance(World);
	if (UNLIKELY(!Settings)) return;

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

	UJUtilsMisc::ShowUI(this, false);

	// this is actually not needed since the game mode is set on the world settings
	// but if we were to need it here it is. we will need to add to the game mode aliases on the map&modes settings, under advanced
	// FString Options = "Game="+ NextLevelMode;
	// UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true, Options);
	UGameplayStatics::OpenLevel(World, FName(*NextLevel), true);
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

	USoundBase* const M = HasDoneSave ? MusicNew.LoadSynchronous() : Music.LoadSynchronous();
	Man->PlayMusic(M, true);
	// TODO play old music if it's not loaded
	// Man->SetFB(1); // TODO fix // dosent 
}
