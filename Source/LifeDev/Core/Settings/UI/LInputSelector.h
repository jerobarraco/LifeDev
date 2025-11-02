// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"

#include "LInputSelector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeySelectedPlus, class ULInputSelector*, Sel, const FInputChord&, Key);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULInputSelector : public UInputKeySelector {
	GENERATED_BODY()
	
public:
	ULInputSelector();

	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void Init(const FInputChord& Key);
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void DeInit();

	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void SetDefault();
	
	// the one used in the player mappable input key stuff very long name persistence that does not work and will crash your game_experimental
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InputName = NAME_None;

	UPROPERTY(BlueprintAssignable)
	FOnKeySelectedPlus OnKeySelectedPlus;

protected:
	virtual void OnWidgetRebuilt() override;

	UFUNCTION()
	void KeySelected(const FInputChord Key);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleBtn = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleText = nullptr;
};
