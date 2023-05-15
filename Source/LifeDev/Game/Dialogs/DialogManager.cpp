// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "DialogManager.h"

#include "DialogUI.h"
#include "LDialogs.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ADialogManager::ADialogManager():Super() {
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	UIClass = UDialogUI::StaticClass();
}

void ADialogManager::Init() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	Dialogs = World->GetSubsystem<ULDialogs>();
	if (!IsValid(Dialogs)) return;

	Dialogs->OnShow.AddUniqueDynamic(this, &ADialogManager::Show);
	Dialogs->OnStop.AddUniqueDynamic(this, &ADialogManager::Stop);
}

void ADialogManager::DeInit() {
	if (IsValid(Dialogs)) {
		Dialogs->OnShow.RemoveAll(this);
	}
	Dialogs = nullptr;

	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;
	
}

void ADialogManager::Show(const FDialog& Diag) {
	UE_LOG(LogTemp, Log, TEXT("DialogManager: Diag %s"), *Diag.Text.ToString());
	if (IsShowing) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to show text when i was already showing."));
	}
	IsShowing = true;

	if (!IsValid(UI)) {
		UE_LOG(LogTemp, Warning, TEXT("DialogManager: UI was not ready"));
		return;
	}

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetWorld()->GetFirstPlayerController());
	UI->SetVisibility(ESlateVisibility::Visible);
	UI->Show(Diag);
}

void ADialogManager::Stop() {
	if (!IsValid(UI)) return;
	HideUI();
}

void ADialogManager::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (!IsShowing) return;

	const float Pressed = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputAnalogKeyState(FKey("E"));
	if (Pressed < .5) return;

	if (!IsValid(Dialogs)) return;
	// reset flag before calling done. done should be the last thing we do.
	IsShowing = false;
	Dialogs->DiagDone();
}

void ADialogManager::HideUI() const {
	if (!IsValid(UI)) return;
	UI->Hide();
	UI->SetVisibility(ESlateVisibility::Hidden);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}

void ADialogManager::BeginPlay() {
	Super::BeginPlay();

	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UDialogUI>(this, Class);
		if (IsValid(UI)) {
			UI->AddToViewport();
			HideUI();
			UI->OnDone.AddUniqueDynamic(this, &ADialogManager::UIFinished);
		}
	}
}

void ADialogManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void ADialogManager::UIFinished() {
	if (!IsValid(Dialogs)) return;
	Dialogs->DiagDone();
}
