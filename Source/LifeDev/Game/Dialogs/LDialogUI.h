// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Diags/DialogUI.h"

#include "LDialogUI.generated.h"

// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ULDialogUI: public UDialogUI {
	GENERATED_BODY()

public:
	ULDialogUI();
};
