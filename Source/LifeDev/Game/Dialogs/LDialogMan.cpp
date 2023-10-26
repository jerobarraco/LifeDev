// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LDialogMan.h"

#include "Diags/DialogUI.h"

ALDialogMan::ALDialogMan():Super() {
	static ConstructorHelpers::FClassFinder<UDialogUI> CUI(TEXT("/Game/LifeDev/Game/Dialogs/UI/W_DialogUI"));
	UIClass = CUI.Succeeded() ? CUI.Class.Get() : UDialogUI::StaticClass();
}
