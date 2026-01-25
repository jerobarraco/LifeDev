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
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings))
		Settings->OnFeatUpdateDiags.RemoveAll(this);

	void FeatUpDlg(const EFeat Feat, const bool Enabled);
	Super::EndPlay(EndPlayReason);
}

void ALDiagMan::Init_Implementation() {
	Super::Init_Implementation();
	Flags = UFlags::Instance(this);
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdateDiags.AddUniqueDynamic(this, &ALDiagMan::FeatUp);
		FeatUp(EFeat::D_AUTO, Settings->GetFeat(EFeat::D_AUTO));
		FeatUp(EFeat::D_SHOW, Settings->GetFeat(EFeat::D_SHOW));
		// FeatUp(EFeat::D_TEXT, Settings->GetFeat(EFeat::D_TEXT));
	}
}

void ALDiagMan::DoEffect_Implementation(const FName& Name, const bool Enabled) {
	Super::DoEffect_Implementation(Name, Enabled);
	// TODO*
	// TODO not sure if this is the best place to listen for effect, or i should use the FeatMan
}

void ALDiagMan::Show_Implementation(const FDiag& Diag) {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Super::Show_Implementation(Diag);

	// if (Diag.Type == EDiagType::SYSTEM) return; 
	
	if (LIKELY(Flags)) Flags->Mod(LDConsts::Flags::Stats::Diags::Shown, 1);
}

void ALDiagMan::FeatUp(const EFeat Feat, const bool Enabled) {
	if (Feat == EFeat::D_SHOW) {
		UseShow = Enabled; // skip dialogs if no feature for it
	} else if (Feat == EFeat::D_AUTO) {
		UseAutoForce = Enabled;
	// } else if (Feat == EFeat::D_TEXT) { // this one is tested on the ui itself.
	}
}

// deprecated it's very redundant. no need to know which dialog got trigged.
// i only need to know which interacts got triggered (and they have the same name (which is good))
// (or item use)
// not necessary. at least until i have other sources of dialog events that arent interacts or items.
// void ALDiagMan::Add_Implementation(const FName Name, const FDiag& Diag) {
// Super::Add_Implementation(Name, Diag);
// if (UNLIKELY(!Flags)) return;

// const FName N(LDConsts::Flags::Diags::AddedPre + Name.ToString());
// Flags->Mod(N,1);
// }
