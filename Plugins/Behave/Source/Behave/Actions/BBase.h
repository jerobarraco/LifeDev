// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "../BTypes.h"

#include "BBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBOnState, const EBState, State);

// base for action behaviors. the idea is that you make your own and each one handles what it needs.
// a top level action, is considered a goal.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UObject {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetState(const EBState New);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EBState GetState() const {return State;};
	
	// called from a bg thread.
	// total cost including children
	// Important. before calling this, ensure you've called plan.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float PlanCost() const;
	
	// called from a bg thread.
	// self cost
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float CostSelf() const;

	// called from a bg thread.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	// this considers itself first, THEN the children.
	// Returns true if the action can/should be done.
	// You are allowed to do some planning here (like choosing which child is your favorite)
	// note in case of actions that have a child like "MoveTo" the MoveTo needs to return true even if it's close to the target (e.g. when performing the action it will be "skipped")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Plan();

	// called from a bg thread.
	// this should only consider itself. and not the children.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanDoSelf() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EBDoRes Do(const float DT);

	UFUNCTION(BlueprintCallable)
	UBBase* GetCurChild();

	UPROPERTY(BlueprintReadWrite, Transient)
	FBOnState OnState;

protected:
	UFUNCTION(BlueprintCallable)
	void StopCurChild();
	UFUNCTION(BlueprintCallable)
	void SetCurChildSate(const EBState New);
	UFUNCTION(BlueprintCallable)
	void StartChild(const int32 I);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UBBase>> Children;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	EBState State = EBState::STOPPED;

	// opt
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 CurChildI = -1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float CostPlanned = 0;
};
