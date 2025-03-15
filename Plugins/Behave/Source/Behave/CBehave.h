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

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void CurStop();

	UFUNCTION(BlueprintCallable)
	void Register(UBBase* const Action);

	UFUNCTION(BlueprintCallable)
	UBBase* GetCur() { return ActionChildCur; };
	UFUNCTION(BlueprintCallable)
	UBBase* NewAction(const TSubclassOf<UBBase>& Class);

	// this is a list of all the goals, main level actions, sorted by priority.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TArray<TObjectPtr<UBBase>> Actions;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TSubclassOf<UBBase>> ActionClasses;

	UPROPERTY(BlueprintReadWrite, Transient)
	FBOnState OnState;
protected:
	virtual void TickComponent(const float DeltaTime, const enum ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Warning. this will run on a bg thread.
	void PlanDo();
	UFUNCTION()
	void PlanStart();
	void PlanDone();
	
	UFUNCTION()
	void ActStateUp(UBBase* const Act, const EBState State);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> ActionsDT = 
		TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Behave/TestActions_DT"));

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
