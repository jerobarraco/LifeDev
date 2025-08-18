// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LComboStr.h"

ULComboStr::ULComboStr() {
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CCB(TEXT("/Game/LifeDev/Core/UI/Combo/LCombo_S"));
	StyleCB = CCB.Object;
	// static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
	// 	CSB(TEXT("/Game/LifeDev/Core/UI/Combo/LScrollBar_S"));
	// StyleScroll = CSB.Object;
	// can't create a slatewidgetasset of type tablerowstyle for some strange reason.
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
	// this is deprecated, and you can't set it after construction
	// if (StyleScroll) {
		// const FScrollBarStyle* const S = StyleItem->GetStyle<FScrollBarStyle>();
		// if (LIKELY(S)) ScrollBarStyle = *S;
	// }
	// should i set the other style manually here? 
}

void ULComboStr::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
