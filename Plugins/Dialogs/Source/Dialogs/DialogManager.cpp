// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "DialogManager.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "DialogUI.h"
#include "Dialogs.h"
#include "JUtils/JMiscUtils.h"

ADialogManager::ADialogManager():Super() {
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Dialogs/Input/IMC_Dialogs"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionSkip(TEXT("/Dialogs/Input/IA_Skip"));
	ActionSkip = CActionSkip.Object;
	UIClass = UDialogUI::StaticClass();
}

void ADialogManager::Init() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	Dialogs = World->GetSubsystem<UDialogs>();
	if (!IsValid(Dialogs)) return;

	Dialogs->OnShow.AddUniqueDynamic(this, &ADialogManager::Show);
	Dialogs->OnDone.AddUniqueDynamic(this, &ADialogManager::Stop);
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

	// we need to actually add and remove so that it doesn't eat the input while not showing
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, true, GetWorld());
	UI->Show(Diag);
}

void ADialogManager::Stop() {
	if (!IsValid(UI)) return;
	HideUI();
}

void ADialogManager::HideUI() const {
	if (!IsValid(UI)) return;
	UI->Hide();
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
}

void ADialogManager::BeginPlay() {
	Super::BeginPlay();

	// bind the action
	UWorld* const World = GetWorld();
	if (ActionSkip) {
		UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(World->GetFirstPlayerController()->InputComponent);
		if (IsValid(Input)) {
			Input->BindAction<ADialogManager>(
				ActionSkip, ETriggerEvent::Triggered, this, &ADialogManager::Skip);
		}
	}
	
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UDialogUI>(this, Class);
		if (IsValid(UI)) {
			UI->AddToViewport();
			HideUI();
		}
	}
}

void ADialogManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
	DeInit();

	// TODO unbind action
	Super::EndPlay(EndPlayReason);
}

void ADialogManager::DiagDone() {
	if (!IsValid(Dialogs)) return;
	IsShowing = false;
	Dialogs->DiagDone();
}

void ADialogManager::Skip() {
	UE_LOG(LogTemp, Log, TEXT("C Skip"));
	if(!IsValid(UI)) return;
	if (UI->IsReady) {
		DiagDone();
		return;
	}
	UI->Skip();
}
