// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "JSlider.h"

void UJSlider::Apply_Implementation() {}

void UJSlider::Load_Implementation() {}

void UJSlider::ResetStyle() {
	if (SliderStyle) {
		const FSliderStyle* const WStyle = SliderStyle->GetStyle<FSliderStyle>();
		SetWidgetStyle(*WStyle);
	}
}

void UJSlider::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}
