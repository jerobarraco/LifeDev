// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LDialogManager.h"

#include "InputMappingContext.h"
#include "InputAction.h"

ALDialogManager::ALDialogManager(): Super() {
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Game/LifeDev/Game/Dialogs/Input/IMC_Dialogs"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionJump(TEXT("/Game/LifeDev/Game/Dialogs/Input/IA_Skip"));
	ActionSkip = CActionJump.Object;
}

