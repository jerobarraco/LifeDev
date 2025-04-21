// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LBLogItemW.h"

#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

#include "Diags/Diags.h"

void ULBLogItemW::SetUp(const FDiag& Diag) {
	if (UNLIKELY(!T_Name || !R_Diag)) {
		UE_LOG(LogTemp, Warning, TEXT("BLogItemW %hs Need a T_Name and R_Diag widgets"), __func__);
		return;
	}

	R_Diag->SetText(Diag.Text);
	
	const UDiags* const Diags = UDiags::Instance(this);
	if (UNLIKELY(!Diags)) return;

	FDiagChar Char;
	if (UNLIKELY(!Diags->GetChar(Diag.CharRow, Char))) return;

	T_Name->SetText(Char.Name);
	T_Name->SetColorAndOpacity(FSlateColor(Char.Color));
}
