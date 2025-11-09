// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"

#include "JUtils/Misc/JUtilsInput.h"

#include "TPInput.generated.h"

enum class EInputType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTPIOnChange, const EInputType, Tgt);

// you need to set this as the default input in the project settings > input
// in theory you can also set is as OverridePlayerInputClass = UTPInput::StaticClass(); on the player controller
// but that didn't work for me
UCLASS(Blueprintable, BlueprintType)
class INVENTORY_API UTPInput : public UEnhancedPlayerInput {
	GENERATED_BODY()

public:
	// triggers when the input type changes. the parameter specifies if it's a GamePad.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	FTPIOnChange OnChange;

protected:
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;

	EInputType Tgt = EInputType::NONE;
};
