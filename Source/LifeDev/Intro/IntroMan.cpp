// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "IntroMan.h"

#include "IntroUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"


AIntroMan::AIntroMan():Super() {
	static ConstructorHelpers::FClassFinder<UIntroUI>
		CUI(TEXT("/Game/LifeDev/Intro/IntroUI_W"));
	UIClass = CUI.Class;
}

void AIntroMan::AddUI() {
	if (!UIClass || !UIClass.Get()) return;

	UI = NewObject<UIntroUI>(this, UIClass.Get());
	if (!IsValid(UI)) return;
	
	UI->AddToViewport();
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	Controller->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, UI);
	UI->OnDone.AddDynamic(this, &AIntroMan::Done);
}

void AIntroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
	
	FString Options = "Game="+ NextLevelMode;
	// TODO the game mode seems to be set, so maybe i don't need this
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true, Options);
}

void AIntroMan::BeginPlay() {
	Super::BeginPlay();
	AddUI();
}
