// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Components/Slider.h"

#include "JSlider.generated.h"

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UJSlider : public USlider {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Apply();

	// Loads and applies the value from the flags
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Load();

	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();

protected:
	virtual void OnWidgetRebuilt() override;

	// FSlider is not exposed to bps!!! WTF?!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> SliderStyle = nullptr;
};
