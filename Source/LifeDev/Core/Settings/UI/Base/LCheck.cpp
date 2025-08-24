// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "LCheck.h"

#include "Components/CheckBox.h"

ULCheck::ULCheck(const FObjectInitializer& O):UUserWidget(O) {
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSC(TEXT("/Game/LifeDev/Core/UI/LCheck_S"));
	StyleCheck = CSC.Object;
}

void ULCheck::ResetStyle() {
	if (bool(StyleCheck) & bool(Check)) {
		// const FButtonStyle* const Style = StyleCheck->GetStyle<FButtonStyle>();
		// if (LIKELY(Style)) Style(*Style);
		const FCheckBoxStyle* const S = StyleCheck->GetStyle<FCheckBoxStyle>();
		if (LIKELY(S)) Check->SetWidgetStyle(*S);
	}
}

void ULCheck::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
