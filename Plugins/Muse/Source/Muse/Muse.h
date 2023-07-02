// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Muse.generated.h"

class AStep;
// World subsystem to deal with muse head band
UCLASS(Blueprintable, Category="Muse")
class MUSE_API UMuse : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();
};

