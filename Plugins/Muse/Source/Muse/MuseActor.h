// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Muse.h"

#include "MuseActor.generated.h"

class UMUse;
UCLASS(Blueprintable, BlueprintType)
class MUSE_API AMuseActor : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetEEG(const TArray<float>& Eeg);
	virtual void SetEEG_Implementation(const TArray<float>& Eeg);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetConfig();
	virtual void SetConfig_Implementation();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UMuse* System = nullptr;
};
