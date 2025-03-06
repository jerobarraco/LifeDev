// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT


#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CBehave.generated.h"

enum class EBDoRes : uint8;
class UBBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();

protected:
	virtual void TickComponent(const float DeltaTime, const enum ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void Dump();
	UFUNCTION()
	void WhatWant();
	UFUNCTION()
	void Do(const float DT);
	void RePlan();

	// leave the object null pls
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UBBase>, TObjectPtr<UBBase>> Behaves;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EBDoRes DoRes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FName> Plan; // TODO try using an ringbuffer
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float PlanVal = 0;
};
