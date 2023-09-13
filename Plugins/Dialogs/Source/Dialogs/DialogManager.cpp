// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "DialogManager.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "JUtils/JMiscUtils.h"

#include "DialogUI.h"
#include "Dialogs.h"
#include "DiagTypes.h" // Log

ADialogManager::ADialogManager():Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Dialogs/Input/IMC_Dialogs"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionSkip(TEXT("/Dialogs/Input/IA_Skip"));
	ActionSkip = CActionSkip.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionBack(TEXT("/Dialogs/Input/IA_Back"));
	ActionBack = CActionBack.Object;
	
	UIClass = UDialogUI::StaticClass();
}

void ADialogManager::Init() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	Dialogs = World->GetSubsystem<UDialogs>();
	if (!IsValid(Dialogs)) return;

	Dialogs->OnShow.AddUniqueDynamic(this, &ADialogManager::Show);
	Dialogs->OnDone.AddUniqueDynamic(this, &ADialogManager::Hide);
}

void ADialogManager::DeInit() {
	if (IsValid(Dialogs)) {
		Dialogs->OnShow.RemoveAll(this);
		Dialogs->OnDone.RemoveAll(this);
	}
	Dialogs = nullptr;

	if (IsValid(UI)) {
		UI->RemoveFromParent();
		UI->OnDone.RemoveAll(this);
	}
	UI = nullptr;
}

void ADialogManager::Show(const FDialog& Diag) {
	UE_LOG(LogTextDialogs, Log, TEXT("DialogManager.Show:"));
	if (IsShowing) {
		UE_LOG(LogTextDialogs, Log, TEXT("Attempted to show text when i was already showing."));
	}

	if (DebugSkip) {
		UE_LOG(LogTextDialogs, Log, TEXT("DialogManager: DebugSkip is set. Skipping."));
		// skip on the next frame to avoid having issues due to call stack
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ADialogManager::UIDiagDone);
		return;
	}

	if (!IsValid(UI)) {
		UE_LOG(LogTextDialogs, Warning, TEXT("DialogManager: UI was not ready"));
		return;
	}
	
	// only set the flag if we are showing something
	IsShowing = true;
	// we need to actually add and remove so that it doesn't eat the input while not showing
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, true, GetWorld());
	UI->Show(Diag);
}

void ADialogManager::Hide() {
	if (!IsShowing) return;
	if (!IsValid(UI)) return;

	IsShowing = false;
	UI->Hide();
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
}

void ADialogManager::BeginPlay() {
	Super::BeginPlay();

	// bind the action
	UWorld* const World = GetWorld();
	UEnhancedInputComponent* const Input = World ? Cast<UEnhancedInputComponent>(World->GetFirstPlayerController()->InputComponent) : nullptr;
	if (IsValid(Input)){
		if (IsValid(ActionSkip)) {
			Input->BindAction<ADialogManager>(
				ActionSkip, ETriggerEvent::Triggered, this, &ADialogManager::Skip);
		}
		if (IsValid(ActionBack)) {
			Input->BindAction<ADialogManager>(
				ActionBack, ETriggerEvent::Triggered, this, &ADialogManager::Back);
		}
	}

	// create ui 
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UDialogUI>(this, Class);
		if (IsValid(UI)) {
			UI->AddToViewport(ZOrder);
			UI->OnDone.AddUniqueDynamic(this, &ADialogManager::UIDiagDone);
			IsShowing = true; // temporarily set, so that it hides.
			Hide();
		}
	}
}

void ADialogManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
	DeInit();

	// TODO unbind actions
	Super::EndPlay(EndPlayReason);
}

void ADialogManager::UIDiagDone() {
	if (!IsValid(Dialogs)) return;
	Dialogs->DiagDone();
}

void ADialogManager::Skip() {
	UE_LOG(LogTextDialogs, Log, TEXT("DialogManager: Skip"));
	if(!IsValid(UI)) return;
	UI->Skip();
}

void ADialogManager::Back() {
	UE_LOG(LogTextDialogs, Log, TEXT("DialogManager: Back"));
	if(!IsValid(UI)) return;
	UI->Back();
}
