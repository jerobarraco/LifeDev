// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

// inspired by ProgressWidgetStyle
#pragma once

#include "Styling/SlateWidgetStyleContainerBase.h"
#include "Styling/SlateStyle.h"

#include "JSlateStyles.generated.h"

// style for a slider widget
UCLASS(BlueprintType, hidecategories=Object, MinimalAPI)
class USliderWidgetStyle : public USlateWidgetStyleContainerBase {
	GENERATED_BODY()

public:
	// The actual data describing the button's appearance.
	UPROPERTY(Category="Style", EditAnywhere, BlueprintReadWrite, meta=(ShowOnlyInnerProperties))
	FSliderStyle SliderStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >(&SliderStyle);
	}
};