// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "JButton.h"
#include "LButton.generated.h"

// todo move to base

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULButton: public UJButton {
	GENERATED_BODY()

public:
	virtual void DoClick_Implementation() override;
};
