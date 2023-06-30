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
	GetWorld()->ServerTravel(NextLevel);
}

void AIntroMan::BeginPlay() {
	Super::BeginPlay();
	AddUI();
}
