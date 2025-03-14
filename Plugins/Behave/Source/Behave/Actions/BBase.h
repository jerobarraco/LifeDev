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
	float Cost() {return 0;};
	// called from a bg thread.
	float CostSelf() {return 0;};

	// called from a bg thread.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	// this considers itself first, THEN the children.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanDo() const;

	// called from a bg thread.
	// this should only consider itself. and not the children.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanDoSelf() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EBDoRes Do(const float DT);

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
};
