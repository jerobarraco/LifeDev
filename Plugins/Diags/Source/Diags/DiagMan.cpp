// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "DiagMan.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "JUtils/Misc/JUtilsSys.h"

#include "DialogUI.h"
#include "Diags.h"
#include "DiagTypes.h" // Log
#include "Kismet/GameplayStatics.h"

ADiagMan::ADiagMan():Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
		DefaultMapping(TEXT("/Diags/Input/IMC_Dialogs"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionSkip(TEXT("/Diags/Input/IA_Skip"));
	ActionSkip = CActionSkip.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionBack(TEXT("/Diags/Input/IA_Back"));
	ActionBack = CActionBack.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction>
		CActionAuto(TEXT("/Diags/Input/IA_Auto"));
	ActionAuto = CActionAuto.Object;
	
	UIClass = UDialogUI::StaticClass();
}

ADiagMan* ADiagMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	return Cast<ADiagMan>(UGameplayStatics::GetActorOfClass(O, StaticClass()));
}

void ADiagMan::Init_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);
	if (UNLIKELY(!IsValid(Diags))) return;

	Diags->OnShow.AddUniqueDynamic(this, &ADiagMan::Show);
	Diags->OnAdd.AddUniqueDynamic(this, &ADiagMan::Add);
	Diags->OnDone.AddUniqueDynamic(this, &ADiagMan::DiagDone);
}

void ADiagMan::DeInit_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);

	if (LIKELY(IsValid(Diags))) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnAdd.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
	}
	Diags = nullptr;

	if (LIKELY(IsValid(UI))) {
		UI->RemoveFromParent();
		UI->OnDone.RemoveAll(this);
	}
	UI = nullptr;
}

void ADiagMan::Add_Implementation(const FName& Name, const FDiag& Diag) {} // Pass: for being overriden


void ADiagMan::Show_Implementation(const FDiag& Diag) {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);

	UE_CLOG(UNLIKELY(IsShowing), LogTextDialogs, Log, TEXT("%hs Attempted to show text when i was already showing."),
		__func__);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	// Idea
	// if (Diag.Type == EDiagType::SYSTEM) {
	// maybe even add a delegate here. for events. some events might need to control WHEN to call Diags->DiagDone (e.g. wait)
	// 	Diags->DiagDone();
	// 	return;
	// }

	if (UNLIKELY(!UseShow)) {
		UE_LOG(LogTextDialogs, Log, TEXT("%hs: DebugSkip is set. Skipping."), __func__);
		// skip on the next frame to avoid having issues due to call stack
		World->GetTimerManager().SetTimerForNextTick(this, &ADiagMan::Hidden);
		return;
	}

	if (UNLIKELY(!IsValid(UI))) {
		UE_LOG(LogTextDialogs, Warning, TEXT("%hs: UI was not ready"), __func__);
		return;
	}

	// only set the flag if we are showing something
	IsShowing = true;
	// we need to actually add and remove so that it doesn't eat the input while not showing
	UJUtilsSys::EInputToggleContext(this, Mapping, InputPrio, true);
	UI->ShowDlg(Diag);
	
	if (UseAutoForce) AutoStart();
}

void ADiagMan::DiagDone_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs IsShowing=%i"), __func__, IsShowing);
	if (UNLIKELY(!IsShowing)) return;
	if (UNLIKELY(!IsValid(UI))) return;

	IsShowing = false;
	UI->Hide();
	UJUtilsSys::EInputToggleContext(this, Mapping, InputPrio, false);
}

void ADiagMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	Diags = World->GetSubsystem<UDiags>();
	// bind the action
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(World);
	if (LIKELY(IsValid(Input))) {
		if (LIKELY(IsValid(ActionSkip)))
			Input->BindAction<ADiagMan>(
				ActionSkip, ETriggerEvent::Triggered, this, &ADiagMan::Skip);
		if (LIKELY(IsValid(ActionBack)))
			Input->BindAction<ADiagMan>(
				ActionBack, ETriggerEvent::Triggered, this, &ADiagMan::Back);
		if (LIKELY(IsValid(ActionAuto))) {
			// TODO this is not working. it's triggering stop right after start (ugh)
			Input->BindAction<ADiagMan>(
				ActionAuto, ETriggerEvent::Started, this, &ADiagMan::AutoStart);
			Input->BindAction<ADiagMan>(
				ActionAuto, ETriggerEvent::Completed, this, &ADiagMan::AutoStop);
		}
	}

	// create ui
	UClass* const Class = UIClass.Get();
	if (UNLIKELY(!IsValid(Class))) return;

	UI = CreateWidget<UDialogUI>(World, Class);
	if (UNLIKELY(!IsValid(UI))) return;

	UI->AddToViewport(ZOrder);
	UI->OnDone.AddUniqueDynamic(this, &ADiagMan::Hidden);
	IsShowing = true; // temporarily set, so that it hides.
	DiagDone();
}

void ADiagMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UJUtilsSys::EInputToggleContext(this, Mapping, InputPrio, false);
	UEnhancedInputComponent* const Input = UJUtilsSys::GetEInput(this);
	if (LIKELY(Input)) Input->ClearBindingsForObject(this);

	DeInit();

	Super::EndPlay(EndPlayReason);
}

void ADiagMan::Hidden_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);
	// clear before Done as it could trigger a new, call show, and maybe we clear something else.
	// actually the subsystem has a protection for that, but it's better to be sure.
	// this case is important when the auto timer runs on a loop
	// disabled. this is kinda not necessary and it's creating issues with the auto with a key.
	// if (UseAutoForce) AutoStop(); 

	if (LIKELY(IsValid(Diags))) Diags->DiagDone();
}

void ADiagMan::Skip_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);
	if(UNLIKELY(!IsValid(UI))) return;

	UI->Skip();
}

void ADiagMan::Back_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);
	if(UNLIKELY(!IsValid(UI))) return;

	AutoStop(); // this is the actual important one, we want to pause skipping if you go back.
	UI->Back();
}

void ADiagMan::AutoStop_Implementation() {
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__);
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	W->GetTimerManager().ClearTimer(AutoTimer);
	AutoTimer.Invalidate();
}

void ADiagMan::AutoStart_Implementation() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	AutoStop(); // for correctness.
	UE_LOG(LogTextDialogs, Log, TEXT("%hs"), __func__); // here because stop logs
	// will set loop if time <2, that's to account for the animation
	W->GetTimerManager().SetTimer(AutoTimer, this, &ADiagMan::Skip, AutoTime, AutoTime < 2);
}
