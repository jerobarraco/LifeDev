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
	UBBase* GetCur() { return TaskLeafCur; };

	// Adds a task to the root tasks. These tasks have a global priority.
	// A Task is only init'ed after adding it here.
	// priority: by default is the last priority (-1). it's the index on the task list.
	UFUNCTION(BlueprintCallable)
	void TaskAdd(UBBase* const Task, const int32 Priority = -1);
	// removes a task. de-init it.
	UFUNCTION(BlueprintCallable)
	int32 TaskRem(const FName Row);

	// creates a new task from a class. does not add it. you need to call TaskAdd.
	UFUNCTION(BlueprintCallable)
	UBBase* TaskNew(const TSubclassOf<UBBase>& Class);

	// loads a task from the dt. does not add it. you need to call TaskAdd.
	// you also need to specify the correct data table
	UFUNCTION(BlueprintCallable)
	UBBase* TaskLoad(const FName Row);

	// this is a list of all the goals (main level tasks), sorted by priority,
	// to be loaded on beginplay.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FName> TasksToLoad;

	// The datatable that defines the tasks to load.
	UPROPERTY(BlueprintReadWrite, EditAnywhere) // TODO change to TSoftObjectPtr to be able to use config
	TSoftObjectPtr<UDataTable> TasksDT = TSoftObjectPtr<UDataTable>(FSoftObjectPath(
		TEXT("/Behave/Test/TestActions_DT.TestActions_DT")));

	// delay between plans
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config)
	float PlanWaitTime = 1;

	// when true. it will keep planning in the bg. interrupting old goals if needed.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config)
	bool UsePlanLoop = false;

	// triggered when a task changes state
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
	void TaskRegister(UBBase* const Action);

	UFUNCTION()
	void TaskStateUp(UBBase* const Act, const EBState State);

	// this is a list of all the goals, main level actions, sorted by priority.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<TObjectPtr<UBBase>> Tasks;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> TaskCur = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> TaskLeafCur = nullptr;

	// this is the chosen goal. (main level task). the root of the plan.
	// only READ this from Game thread. only write from plan.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> TaskPlan = nullptr;

	bool IsPlanning = false;
};

