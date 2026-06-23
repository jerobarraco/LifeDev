// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "Blueprint/UserWidget.h"

#include "LCheck.generated.h"

class USlateWidgetStyleAsset;
class UTextBlock;
class UCheckBox;

// basic checkbox
UCLASS()
class LIFEDEV_API ULCheck : public UUserWidget {
	GENERATED_BODY()
public:
	ULCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintNativeEvent, CallInEditor, meta=(UnsafeDuringActorConstruction, ForceAsFunction))
	void Load();
	UFUNCTION(BlueprintNativeEvent, CallInEditor, meta=(UnsafeDuringActorConstruction, ForceAsFunction))
	void Apply();

	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SetChecked(const bool NewChecked) const;
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	bool GetChecked() const;
	
	// please only set before WidgetRebuild is called. or call ResetStyle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FText Label;
	// EditAnywhere allows me to edit when placed on parent widgets

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UCheckBox> Check = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Text = nullptr;
	// made public because they get accessed by other means

protected:
	virtual void OnWidgetRebuilt() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleCheck = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleText = nullptr;
};
