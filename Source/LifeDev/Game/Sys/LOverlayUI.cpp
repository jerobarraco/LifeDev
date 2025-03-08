// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "LOverlayUI.h"

ULOverlayUI::ULOverlayUI() :Super(){
	// this is getting the build stuck
	// static ConstructorHelpers::FObjectFinderOptional<UMaterialParameterCollection>
		// CMPC(TEXT("/Game/LifeDev/Game/Overlays/UI/Mats/Diag_MPC.Diag_MPC"), LOAD_Async);
	// TextMPC = CMPC.Succeeded() ? CMPC.Get() : nullptr;
}