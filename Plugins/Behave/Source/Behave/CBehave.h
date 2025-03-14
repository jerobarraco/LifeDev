// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CBehave.generated.h"

// https://www.youtube.com/watch?v=gm7K68663rA#
// it's loosely based on the first person And the comment about Layered GOAP. 

class UBBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Behave), meta=(BlueprintSpawnableComponent))
class BEHAVE_API UCBehave: public UActorComponent {
	GENERATED_BODY()
public:
	UCBehave();
	void CurStop();
	UPROPERTY(BlueprintReadWrite)
	TArray<TSubclassOf<UBBase>> ActionClasses;

protected:
	virtual void TickComponent(const float DeltaTime, const enum ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Warning. this will run on a bg thread.
	void PlanDo();
	void PlanStart();
	void PlanDone();
	// this is a list of all the goals, main level actions, sorted by priority.
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UBBase>> Actions;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> ActionCur;
	// this is faulty. our plan is a tree, so we only need access to the root.
	// only READ this from Game thread. only write from plan.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBBase> Planned;

	bool IsPlanning = false;
};
