// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LCheck.generated.h"

class UTextBlock;
class UCheckBox;

// basic checkbox
UCLASS()
class LIFEDEV_API ULCheck : public UUserWidget {
	GENERATED_BODY()
public:
	ULCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintNativeEvent, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Load();
	UFUNCTION(BlueprintNativeEvent, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Apply();
	UFUNCTION(BlueprintNativeEvent, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Reset();

	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();

protected:
	virtual void OnWidgetRebuilt() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleCheck = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UCheckBox> Check = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Text = nullptr;
};
