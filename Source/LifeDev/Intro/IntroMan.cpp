// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroMan.h"

#include "Kismet/GameplayStatics.h"

#include "JUtils/Misc/JUtilsMisc.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "IntroUI.h"

AIntroMan::AIntroMan():Super() {
	static ConstructorHelpers::FClassFinder<UIntroUI>
		CUI(TEXT("/Game/LifeDev/Intro/UI/IntroUI_W"));
	UIClass = CUI.Class;
}

void AIntroMan::AddUI() {
	if (!UIClass || !UIClass.Get()) return;

	UWorld* const World = GetWorld();
	if (!World) return;

	UI = CreateWidget<UIntroUI>(World, UIClass.Get());
	if (UNLIKELY(!IsValid(UI))) return;
	
	UI->AddToViewport();
	UI->OnDone.AddDynamic(this, &AIntroMan::Done);

	UJUtilsMisc::ShowUI(this, true, UI, false);
}

void AIntroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	const UWorld* const World = GetWorld();

	const ULSettings* const Settings = ULSettings::Instance(World);
	if (!Settings) return;

	const int32 ChapterID = Settings->CurrentChapter();
	// -1 because we never actually save that chapter.
	constexpr int32 ChapMax = UJUtilsMisc::ArraySize(LDConsts::Feats::ChapFeats) -1;

	if (UNLIKELY(ChapterID >= ChapMax)) {
		UE_LOG(LogTemp, Log, TEXT("Current save is beyond the max chapter."));
		static const FText TheEnd(
			NSLOCTEXT("Intro", "MaxChapterReached",
				"This save is at the current max chapter.\n"
					"Maybe in a next release i'll add more.\n"
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
	if (LIKELY(UI)) UI->OnDone.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}
