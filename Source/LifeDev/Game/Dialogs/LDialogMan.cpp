// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LDialogMan.h"

#include "Diags/DialogUI.h"
#include "Inventory/Flags.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

ALDialogMan::ALDialogMan():Super() {
	static ConstructorHelpers::FClassFinder<UDialogUI>
		CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_DialogUI"));
	UIClass = CUI.Succeeded() ? CUI.Class.Get() : UDialogUI::StaticClass();
}

void ALDialogMan::BeginPlay() {
	Super::BeginPlay();
	Flags = UFlags::Instance(this);
}

void ALDialogMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Flags = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALDialogMan::Show_Implementation(const FDialog& Diag) {
	Super::Show_Implementation(Diag);
	
	if (LIKELY(Flags)) Flags->Mod(LDConsts::Flags::Stats::DiagShown, 1);
}
