// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol
// SPDX-License-Identifier: MIT

#pragma once

#include "EnhancedPlayerInput.h"

#include "JUtilsInput.h"

#include "JPInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJPIOnChange, const EInputType, Tgt);

// you need to set this as the default input in the project settings > input
// in theory you can also set is as OverridePlayerInputClass = UTPInput::StaticClass(); on the player controller
// but that didn't work for me
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UJPInput : public UEnhancedPlayerInput {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EInputType GetLastType() const { return LastType; }

	// triggers when the input type changes. the parameter specifies if it's a GamePad.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FJPIOnChange OnChange;

protected:
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;

	EInputType LastType = EInputType::NONE;
};
