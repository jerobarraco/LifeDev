// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LComboStr.h"

#include "UObject/ConstructorHelpers.h"

#include "JUtilsUI.h"

ULComboStr::ULComboStr() {
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CCB(TEXT("/Game/LifeDev/Core/UI/Combo/LCombo_S"));
	StyleCB = CCB.Object;

	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSB(TEXT("/Game/LifeDev/Core/UI/LScrollBar_S"));
	StyleScroll = CSB.Object;

	static ConstructorHelpers::FObjectFinder<UJTableRowStyle>
		CST(TEXT("/Game/LifeDev/Core/UI/Combo/LComboItem_S"));
	StyleItem = CST.Object;

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
	if (StyleItem) SetItemStyle(StyleItem->Style);
	// if (StyleItem) {
	// 	const FTableRowStyle* const S = StyleItem->GetStyle<FTableRowStyle>();
	// 	if (LIKELY(S)) SetItemStyle(*S);
	// }
	// this is deprecated, and you can't set it after construction
	if (StyleScroll) {
		const FScrollBarStyle* const S = StyleScroll->GetStyle<FScrollBarStyle>();
		// ReSharper disable once CppDeprecatedEntity
		if (LIKELY(S)) ScrollBarStyle = *S;
	}
	// should i set the other style manually here? 
}

void ULComboStr::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
