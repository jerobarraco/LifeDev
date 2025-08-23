// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Components/ComboBoxString.h"
#include "LComboStr.generated.h"


class UJTableRowStyle;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULComboStr : public UComboBoxString {
	GENERATED_BODY()

public:
	ULComboStr();

	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();

protected:
	virtual void OnWidgetRebuilt() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleCB = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UJTableRowStyle> StyleItem = nullptr;
	// TObjectPtr<USlateWidgetStyleAsset> StyleItem = nullptr; // until epic implements this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleScroll = nullptr;
};
