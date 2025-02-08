// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CPuzzle.h"
#include "Puzzle.generated.h"

class AInteract;

// Note: not setting the Set* functions as Const or bps will show them as pure and won't be able to be executed (facepalm).
// if they were, they shouldn't be pure anyway.

// Base class for Puzzles that can optionally trigger an Interact
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API APuzzle: public AActor {
	GENERATED_BODY()

public:
	APuzzle();
	
	// sets the states on each registered interact.
	// Call on, or after, begin play (but not before).
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStates(const TArray<int32>& States) {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetStates(States); }
	}
	
	// sets the states on each registered interact.
	// Use on PostLoad (or BeginPlay) (if you've set the interacts on the editor's world outliner
	// unless you've set the reference of the CPuzzle->Interacts on the constructor).
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetLocks(const TArray<bool>& Locks) {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetLocks(Locks); }
	}

	// Set the interact pieces to active. Don't call during construction.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetActives(const bool NewEnabled) {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetActives(NewEnabled); }
	}

	// Set the interact pieces to auto activate. Call only on constructor.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAutoActives(const bool NewEnabled) {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetAutoActives(NewEnabled); }
	}

	// sets "DisableWhileAnims" on all the interacts.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDisableWhileAnims(const bool NewDisabled) {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetDisableWhileAnims(NewDisabled); }
	}

	// call to reset the puzzle. Override DoReset to do custom logic.
	UFUNCTION(BlueprintCallable)
	virtual void Reset() override;

	// Automatically reset the puzzle on failure.
	// works only on SEQUENCE since combination can't fail.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reset")
	bool ResetOnFail = false;

	// Will reset when reaching this time without interacting with it
	// Disabled if <=0 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reset")
	float ResetTimeout = 0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void ClearTimer();
	
	// called when the puzzle is done. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Done(const bool IsOk);
	virtual void Done_Implementation(const bool IsOk);

	// called when the puzzle is updated. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Update();
	virtual void Update_Implementation();
	
	// called when the puzzle is reset. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DoReset();
	virtual void DoReset_Implementation() {};


	// Interact to *trigger* on Done. It will force unlock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	TObjectPtr<AInteract> DoneInter = nullptr;
	
	// Actor to reward on Done. Can be an interact, it will use fade, but won't trigger.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	TObjectPtr<AActor> DoneActor = nullptr;

	/// subobjects
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCPuzzle> CPuzzle = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Root = nullptr;

	/// transient

	FTimerHandle ResetTimer;
};
