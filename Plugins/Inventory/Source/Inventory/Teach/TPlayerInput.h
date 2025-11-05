// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "TPlayerInput.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INVENTORY_API UTPlayerInput : public UEnhancedPlayerInput {
	GENERATED_BODY()

protected:
	virtual bool InputKey(const FInputKeyEventArgs& Params) override;
};
