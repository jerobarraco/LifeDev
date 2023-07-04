// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "OutroMan.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "OutroUI.h"

AOutroMan::AOutroMan():Super() {
	static ConstructorHelpers::FClassFinder<UOutroUI>
		CUI(TEXT("/Game/LifeDev/Outro/OutroUI_W"));
	UIClass = CUI.Class;
}

void AOutroMan::AddUI() {
	if (!UIClass || !UIClass.Get()) return;

	UI = NewObject<UOutroUI>(this, UIClass.Get());
	if (!IsValid(UI)) return;
	
	UI->AddToViewport();
	UI->OnDone.AddUniqueDynamic(this, &AOutroMan::Done);
	UI->OnQuit.AddUniqueDynamic(this, &AOutroMan::Quit);
	// enable ui controls
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Controller->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(Controller, UI);
}

void AOutroMan::Quit() {
	UKismetSystemLibrary::QuitGame(
		GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AOutroMan::Done() {
	// GetWorld()->ServerTravel(NextLevel);
	// https://stackoverflow.com/a/50205038
	// https://www.reddit.com/r/unrealengine/comments/bf46lz/comment/elaskww/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button

	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	Controller->bShowMouseCursor = false;

	// this is actually not needed since the game mode is set on the world settings
	// but if we were to need it here it is. we will need to add to the game mode aliases on the map&modes settings, under advanced
	// FString Options = "Game="+ NextLevelMode;
	// UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true, Options);
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GameLevel), true);
}

void AOutroMan::BeginPlay() {
	Super::BeginPlay();
	AddUI();
}
