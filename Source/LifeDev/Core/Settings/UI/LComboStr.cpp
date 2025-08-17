// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LComboStr.h"

ULComboStr::ULComboStr() {
	// static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
	// 	CS(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall_S"));
	// static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
	// 	CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	// StyleCB = CS.Object;
	// StyleItem = CST.Object;
}

void ULComboStr::ResetStyle() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	if (StyleCB) {
		const FComboBoxStyle* const S = StyleCB->GetStyle<FComboBoxStyle>();
		if (LIKELY(!S)) SetWidgetStyle(*S);
	}
	if (StyleItem) {
		const FTableRowStyle* const S = StyleItem->GetStyle<FTableRowStyle>();
		if (LIKELY(!S)) SetItemStyle(*S);
	}
}

void ULComboStr::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
