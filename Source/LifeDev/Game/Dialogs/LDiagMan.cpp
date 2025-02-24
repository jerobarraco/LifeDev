// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LDiagMan.h"

#include "Diags/Diags.h"
#include "Kismet/GameplayStatics.h"

#include "Diags/DialogUI.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALDiagMan::ALDiagMan():Super() {
	static ConstructorHelpers::FClassFinder<UDialogUI>
		CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_LDialogUI"));
	UIClass = LIKELY(CUI.Succeeded()) ? CUI.Class.Get() : UDialogUI::StaticClass();
}

ALDiagMan* ALDiagMan::InstanceL(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	const UWorld* const W = O->GetWorld();
	const ALGGameMode* const Mode = LIKELY(W)
		? Cast<ALGGameMode>(W->GetAuthGameMode()) : nullptr;
	if (LIKELY(Mode)) return Mode->DiagMan;

	// probably not necessary but for completion.
	return Cast<ALDiagMan>(UGameplayStatics::GetActorOfClass(O, StaticClass()));
}

void ALDiagMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Flags = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALDiagMan::Init_Implementation() {
	Super::Init_Implementation();
	Flags = UFlags::Instance(this);
}

void ALDiagMan::Add_Implementation(const FName Name, const FDiag& Diag) {
	Super::Add_Implementation(Name, Diag);
}

void ALDiagMan::Show_Implementation(const FDiag& Diag) {
	const UWorld* const W = GetWorld();
	if (LIKELY(!W)) return;

	Super::Show_Implementation(Diag);
	if (LIKELY(Flags)) Flags->Mod(LDConsts::Flags::Stats::Diags::Shown, 1);

	const bool UseAuto = ULSettings::GetFeatS(W, EFeat::D_AUTO);
	if (!UseAuto) return;

	AutoClear(); // for correctness.
	// will set loop if time <2, that's to account for the animation
	W->GetTimerManager().SetTimer(AutoTimer, this, &ALDiagMan::Skip, AutoTime, AutoTime < 2);
}

void ALDiagMan::Hidden_Implementation() {
	// clear before super as it could trigger a new, call show, and maybe we clear something else.
	// actually the dialog subsystem has a protection for that, but it's better to be sure.
	// this case it's important when the auto timer runs on a loop
	AutoClear();
	Super::Hidden_Implementation();
}

void ALDiagMan::Back_Implementation() {
	Super::Back_Implementation();
	AutoClear(); // this is the actual important one, we want to pause skipping if you go back.
}

void ALDiagMan::AutoClear() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	W->GetTimerManager().ClearTimer(AutoTimer);
	AutoTimer.Invalidate();
}
