// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CBehave.generated.h"

enum class EBDoRes: uint8;
class UBBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCBehaveDo, const FName&, Token);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();

	UFUNCTION(BlueprintCallable)
	void ReactState(const float DT, const FName& Name, const float V=0);
	UPROPERTY(BlueprintAssignable, Transient)
	FCBehaveDo OnDo;

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
	void PlanCheck();
	void RePlan();
	void ReactAllStates(float DeltaTime);
	
	// leave the object null pls
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UBBase>, TObjectPtr<UBBase>> Behaves;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EBDoRes DoRes;

	// a plan is always for ONE want, a very very simple plan
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FName> Plan;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float PlanVal = 0; // TODO is this even useful? at least for debug on outliner
};
