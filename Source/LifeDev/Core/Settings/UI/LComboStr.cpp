// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LComboStr.h"

ULComboStr::ULComboStr() {static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CS(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall_S"));
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	StyleCB = CS.Object;
	if (StyleCB) {
		const FComboBoxStyle* const S = CS.Object->GetStyle<FComboBoxStyle>();
		SetWidgetStyle(*S);
	}

	StyleItem = CST.Object;
	if (StyleItem) {
		const FTableRowStyle* const S = CST.Object->GetStyle<FTableRowStyle>();
		SetItemStyle(*S);
	}
	// TODO have these
	// SetWidgetStyle()
	// SetItemStyle()
}
