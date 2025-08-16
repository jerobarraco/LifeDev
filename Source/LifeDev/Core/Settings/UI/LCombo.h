// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Components/ComboBox.h"
#include "LCombo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeySelectedPlus, class ULInputSelector*, Sel, const FInputChord&, Key);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULCombo : public UComboBox{
	GENERATED_BODY()
	
public:
	
};
