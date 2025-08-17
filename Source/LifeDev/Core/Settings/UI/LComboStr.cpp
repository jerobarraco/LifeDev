// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LComboStr.h"

ULComboStr::ULComboStr() {
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CCB(TEXT("/Game/LifeDev/Core/UI/Combo/LCombo_S"));
	StyleCB = CCB.Object;
	// static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
	// 	CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	// StyleItem = CST.Object;
}

void ULComboStr::ResetStyle() {
	if (StyleCB) {
		const FComboBoxStyle* const S = StyleCB->GetStyle<FComboBoxStyle>();
		if (LIKELY(S)) SetWidgetStyle(*S);
	}
	if (StyleItem) {
		const FTableRowStyle* const S = StyleItem->GetStyle<FTableRowStyle>();
		if (LIKELY(S)) SetItemStyle(*S);
	}
}

void ULComboStr::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
