// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "LDialogUI.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ULDialogUI::ULDialogUI() :Super(){
	// this is getting the build stuck
	// static ConstructorHelpers::FObjectFinderOptional<UMaterialParameterCollection>
		// CMPC(TEXT("/Game/LifeDev/Game/Dialogs/UI/Mats/Diag_MPC.Diag_MPC"), LOAD_Async);
	// TextMPC = CMPC.Succeeded() ? CMPC.Get() : nullptr;
	TextMPC = nullptr;
}

void ULDialogUI::NativeDestruct() {
	Super::NativeDestruct();
	TextMPC = nullptr;
	TextMPCI = nullptr;
}

void ULDialogUI::NativeConstruct() {
	Super::NativeConstruct();
	
	if (IsValid(TextMPC))
		TextMPCI = GetWorld()->GetParameterCollectionInstance(TextMPC); 
}

void ULDialogUI::SetTextMatProgress(float T) {
	static FName PName("TextProg");
	if (IsValid(TextMPCI)) TextMPCI->SetScalarParameterValue(PName, T);
}
