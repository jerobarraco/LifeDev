// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetControlUI.generated.h"

class UInputAction;
class USlider;
class ULInputSelector;
class UJButton;

// baseclass for the control settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetControlUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	ULSetControlUI();
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void ApplyKeyNames();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()
	void SetDefaults(const int32 Id);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void ResetSelectors();
	// it only resets the action, it does not load the slider
	UFUNCTION(BlueprintCallable)
	void SetModifier(UInputAction* const Action, const double Val);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ResetModifier(UInputAction* const Action) { SetModifier(Action, 1); }

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TObjectPtr<ULInputSelector>> Selectors;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UInputAction> IA_Move = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UInputAction> IA_Look = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BDefaults = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SMove = nullptr; // set on the bp, todo move here
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLook = nullptr;

};
