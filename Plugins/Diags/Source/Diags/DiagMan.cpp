// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "DiagMan.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "JUtils/Misc/JUtilsSys.h"

#include "DialogUI.h"
#include "Diags.h"
#include "DiagTypes.h" // Log

ADiagMan::ADiagMan():Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Diags/Input/IMC_Dialogs"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionSkip(TEXT("/Diags/Input/IA_Skip"));
	ActionSkip = CActionSkip.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionBack(TEXT("/Diags/Input/IA_Back"));
	ActionBack = CActionBack.Object;
	
	UIClass = UDialogUI::StaticClass();
}

void ADiagMan::Init_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: Init"));
	if (!IsValid(Diags)) return;

	Diags->OnShow.AddUniqueDynamic(this, &ADiagMan::Show);
	Diags->OnDone.AddUniqueDynamic(this, &ADiagMan::Hide);
}

void ADiagMan::DeInit_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: DeInit"));

	if (IsValid(Diags)) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
	}
	Diags = nullptr;

	if (IsValid(UI)) {
		UI->RemoveFromParent();
		UI->OnDone.RemoveAll(this);
	}
	UI = nullptr;
}

void ADiagMan::Show_Implementation(const FDialog& Diag) {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan.Show:"));
	if (IsShowing)
		UE_LOG(LogTextDialogs, Log, TEXT("Attempted to show text when i was already showing."));

	if (DebugSkip) {
		UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: DebugSkip is set. Skipping."));
		// skip on the next frame to avoid having issues due to call stack
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ADiagMan::UIDiagDone);
		return;
	}

	if (!IsValid(UI)) {
		UE_LOG(LogTextDialogs, Warning, TEXT("DiagMan: UI was not ready"));
		return;
	}
	
	// only set the flag if we are showing something
	IsShowing = true;
	// we need to actually add and remove so that it doesn't eat the input while not showing
	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, true);
	UI->ShowDlg(Diag);
}

void ADiagMan::Hide_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: UIDiagDone IsShowing=%i"), IsShowing);
	if (!IsShowing) return;
	if (!IsValid(UI)) return;

	IsShowing = false;
	UI->Hide();
	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, false);
}

void ADiagMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	Diags = World->GetSubsystem<UDiags>();
	// bind the action
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(World);
	if (IsValid(Input)){
		if (IsValid(ActionSkip))
			Input->BindAction<ADiagMan>(
				ActionSkip, ETriggerEvent::Triggered, this, &ADiagMan::Skip);
		if (IsValid(ActionBack))
			Input->BindAction<ADiagMan>(
				ActionBack, ETriggerEvent::Triggered, this, &ADiagMan::Back);
	}

	// create ui 
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = CreateWidget<UDialogUI>(World, Class);
		if (IsValid(UI)) {
			UI->AddToViewport(ZOrder);
			UI->OnDone.AddUniqueDynamic(this, &ADiagMan::UIDiagDone);
			IsShowing = true; // temporarily set, so that it hides.
			Hide();
		}
	}
}

void ADiagMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UJUtilsSys::ToggleMapping(this, Mapping, InputPrio, false);
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(this);
	if (Input) Input->ClearBindingsForObject(this);

	DeInit();

	Super::EndPlay(EndPlayReason);
}

void ADiagMan::UIDiagDone() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: UIDiagDone"));
	if (!IsValid(Diags)) return;
	Diags->DiagDone();
}

void ADiagMan::Skip() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: Skip"));
	if(!IsValid(UI)) return;
	UI->Skip();
}

void ADiagMan::Back() {
	UE_LOG(LogTextDialogs, Log, TEXT("DiagMan: Back"));
	if(!IsValid(UI)) return;
	UI->Back();
}
