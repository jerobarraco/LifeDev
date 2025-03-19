// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "BTypes.h"

#include "BBase.generated.h"

class UCBehave;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBOnState, UBBase* const, Act, const EBState, State);

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
	float Cost();

	// called from a bg thread.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	// this considers itself first, THEN the children.
	// Returns true if the action can/should be done.
	// You are allowed to do some planning here (like choosing which child is your favorite)
	// note in case of actions that have a child like "MoveTo" the MoveTo needs to return true even if it's close to the target (e.g. when performing the action it will be "skipped")
	// ensure you need to set CostPlanned inside this.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	bool Plan();

	UFUNCTION(BlueprintCallable)
	EBDoRes Do(const float DT);
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Start() {SetState(EBState::STARTED);};
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Stop() {SetState(EBState::STOPPED);};

	UFUNCTION(BlueprintCallable)
	UBBase* GetCurChild() const;

	// for you to use however you want.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ID = NAME_None;

	UPROPERTY(BlueprintReadWrite)
	bool IsLooped = false;

	UPROPERTY(BlueprintReadWrite, Transient)
	FBOnState OnState;

protected:
	// override this to do your job
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	EBDoRes DoSelf(const float DT);
	// override this one if needed. will/should/could be called during Plan AND Cost (called during Re-Plan).
	// always in a bg thread.
	// you have to make sure to handle the case where the task is stopped or running.
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void CostCalc();

	UFUNCTION(BlueprintCallable)
	void StopCurChild();
	UFUNCTION(BlueprintCallable)
	void SetCurChildSate(const EBState New);
	UFUNCTION(BlueprintCallable)
	void StartChild(const int32 I);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void Init(UCBehave* B); // Native can't use const
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void DeInit();
	
	// beware if you change it during runtime
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UBBase>> Children; // TODO make protected and friendly

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	EBState State = EBState::STOPPED;

	// opt
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 CurChildI = -1;
	
	// You need to set this inside CostCalc.
	// And probably call CostCalc inside Plan, or set it directly on plan.
	// Or before, if the cost is constant.
	// Make sure to set to FLT_MAX in case of error.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float CostCur = 0;

	UPROPERTY(Transient)
	TObjectPtr<UCBehave> Behave = nullptr;

	friend class UCBehave;
};
