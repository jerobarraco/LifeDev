// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "OutroMan.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "JUtilsUI.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"

#include "OutroUI.h"

AOutroMan::AOutroMan():Super() {
	static ConstructorHelpers::FClassFinder<UOutroUI>
		CUI(TEXT("/Game/LifeDev/Outro/OutroUI_W"));
	UIClass = CUI.Class;
}

void AOutroMan::AddUI() {
	if (!UIClass || !UIClass.Get()) return;

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UI = CreateWidget<UOutroUI>(World, UIClass.Get());
	if (UNLIKELY(!IsValid(UI))) return;
	
	UI->AddToViewport();
	UI->OnDoneVal.AddUniqueDynamic(this, &AOutroMan::Done);

	const ULSettings* const Settings = ULSettings::Instance(World);
	if (LIKELY(Settings) && Settings->Save)
		UI->SetFlags(Settings->Save->GetFlags());

	UJUtilsUI::ShowUI(this, true, UI);
}

void AOutroMan::Quit() const {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UKismetSystemLibrary::QuitGame(
		World, World->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AOutroMan::Retry() const {
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

void AOutroMan::Done(const int32 RetVal) {
	if (RetVal == 0) Retry();
	else if (RetVal ==1) Quit();
}

void AOutroMan::BeginPlay() {
	Super::BeginPlay();
	AddUI();
}

void AOutroMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(UI)) UI->OnDone.RemoveAll(this);
	UI = nullptr;
	Super::EndPlay(EndPlayReason);
}
