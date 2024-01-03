// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroMan.h"

#include "IntroUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "JUtils/JMiscUtils.h"
#include "Kismet/GameplayStatics.h"


AIntroMan::AIntroMan():Super() {
	static ConstructorHelpers::FClassFinder<UIntroUI>
		CUI(TEXT("/Game/LifeDev/Intro/IntroUI_W"));
	UIClass = CUI.Class;
}

void AIntroMan::AddUI() {
	if (!UIClass || !UIClass.Get()) return;

	UWorld* const World = GetWorld();
	if (!World) return;

	UI = CreateWidget<UIntroUI>(World, UIClass.Get());
	if (!IsValid(UI)) return;
	
	UI->AddToViewport();
	UI->OnDone.AddDynamic(this, &AIntroMan::Done);

	UJMiscUtils::ShowUI(true, World, UI, false);
}

void AIntroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	UWorld* const World = GetWorld();
	UJMiscUtils::ShowUI(false, World);

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
	if (UI) {
		UI->OnDone.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}
