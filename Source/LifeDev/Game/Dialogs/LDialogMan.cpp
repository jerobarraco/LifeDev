// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LDialogMan.h"

#include "Diags/Diags.h"
#include "Diags/DialogUI.h"
#include "Inventory/Flags.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"

ALDialogMan::ALDialogMan():Super() {
	static ConstructorHelpers::FClassFinder<UDialogUI>
		CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_DialogUI"));
	UIClass = CUI.Succeeded() ? CUI.Class.Get() : UDialogUI::StaticClass();
}

void ALDialogMan::BeginPlay() {
	Super::BeginPlay();

	UDiags* Diags = UDiags::Instance(this);
	if (Diags) Diags->OnShow.AddUniqueDynamic(this, &ALDialogMan::DiagShown);
}

void ALDialogMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UDiags* Diags = UDiags::Instance(this);
	if (Diags) Diags->OnShow.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

void ALDialogMan::DiagShown(const FDialog& Diag) {
	UFlags* Flags = UFlags::Instance(this);
	if (Flags) Flags->Mod(LDConsts::Flags::Play::DiagShown, 1);
}
