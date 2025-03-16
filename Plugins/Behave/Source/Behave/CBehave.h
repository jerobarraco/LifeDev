// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BTypes.h"
#include "Actions/BBase.h"

#include "CBehave.generated.h"

// https://www.youtube.com/watch?v=gm7K68663rA#
// it's loosely based on the first person And the comment about Layered GOAP. 

class UBBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave),
	Config=Behave, DefaultConfig, meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void CurStop();

	// will try to plan again (in the bg). will interrupt current action.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Plan();
	
	UFUNCTION(BlueprintCallable)
	UBBase* GetCur() { return ActionChildCur; };

	// priority, by default is the last priority (-1)
	UFUNCTION(BlueprintCallable)
	void AddAction(UBBase* const Action, const int32 Priority = -1);
	UFUNCTION(BlueprintCallable)
	int32 RemAction(const FName Row);

	// creates a new action from a class. does not add it. you need to call AddAction.
	UFUNCTION(BlueprintCallable)
	UBBase* NewAction(const TSubclassOf<UBBase>& Class);

	// loads an action from the dt. does not add it. you need to call AddAction
	UFUNCTION(BlueprintCallable)
	UBBase* LoadAction(const FName Row);

	// this is a list of all the goals, main level actions, sorted by priority.
	// to be loaded on beginplay
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FName> ActionsToLoad;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config)
	float PlanWaitTime = 1;

	UPROPERTY(BlueprintReadWrite, Transient)
	FBOnState OnState;

protected:
	virtual void TickComponent(const float DeltaTime, const enum ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Warning. this will run on a bg thread.
	void PlanDo();
	void PlanDone();

	friend void UBBase::Init(UCBehave*); // doesn't work as expected.
	friend class UBBase;
	void ActRegister(UBBase* const Action);

	UFUNCTION()
	void ActStateUp(UBBase* const Act, const EBState State);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> ActionsDT = nullptr;

	// this is a list of all the goals, main level actions, sorted by priority.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<TObjectPtr<UBBase>> Actions;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> ActionCur;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> ActionChildCur;
	// this is faulty. our plan is a tree, so we only need access to the root.
	// only READ this from Game thread. only write from plan.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> Planned;

	bool IsPlanning = false;
};

