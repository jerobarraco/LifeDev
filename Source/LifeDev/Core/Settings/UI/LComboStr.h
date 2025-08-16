// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Components/ComboBoxString.h"
#include "LComboStr.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeySelectedPlus, class ULInputSelector*, Sel, const FInputChord&, Key);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULComboStr : public UComboBoxString {
	GENERATED_BODY()
	
public:
	ULComboStr();
	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle() { };// SetStyle(BtnStyle, TextStyle); }

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleCB = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleItem = nullptr;
};
