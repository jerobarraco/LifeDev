// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "LDialogUI.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ULDialogUI::ULDialogUI() :Super(){
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Dialogs/UI/Mats/Diag_MPC.Diag_MPC"));
	TextMPC = CMPC.Succeeded() ? CMPC.Object : nullptr;
}

void ULDialogUI::NativeDestruct() {
	Super::NativeDestruct();
	TextMPC = nullptr;
	TextMPCI = nullptr;
}

void ULDialogUI::NativeConstruct() {
	Super::NativeConstruct();
	
	if (IsValid(TextMPC)) {
		TextMPCI = GetWorld()->GetParameterCollectionInstance(TextMPC); 
	}
}

void ULDialogUI::SetTextMatProgress(float T) {
	static FName PName("FadeProgress");
	if (IsValid(TextMPCI)) {
		TextMPCI->SetScalarParameterValue(PName, T);
	}
}
