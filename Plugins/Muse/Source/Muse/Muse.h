// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "OSCServer.h"
#include "Subsystems/WorldSubsystem.h"

#include "Muse.generated.h"

class AStep;
// World subsystem to deal with muse head band
UCLASS(Blueprintable, Category="Muse")
class MUSE_API UMuse : public UWorldSubsystem
{
	GENERATED_BODY()

	UMuse();

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init(const FString& Address, int32 Port);
	void Init_Implementation(const FString& Address, int32 Port);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();

	UPROPERTY(BlueprintReadOnly, Transient)
	UOSCServer* Server = nullptr;
};

