// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CPuzzle.h"
#include "InteractAnim.h"

#include "Puzzle.generated.h"

class AInteract;

// Note: Set* functions as Const need blueprintpure=false or bps will show them as pure and won't be able to be executed (facepalm).

// Note to Jero and maybe users. This class is "abandoned", i'm using my own class due to problems of inheritance.
// (i have an intermediat class LInteract child of AInteractAnim, but then i can't make my own LPuzzle child of LInteract and APuzzle due to ue limitation).
// i'll leave this here as an example and as basic usage. but i've created a _copy_ and i'll use THAT instead (LPuzzle)

// Base class for Puzzles.
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API APuzzle: public AInteractAnim {
	GENERATED_BODY()

public:
	APuzzle();
	
	// sets the states on each registered interact.
	// Call on, or after, begin play (but not before).
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetStates(const TArray<int32>& States) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetStates(States); }
	}
	
	// sets the states on each registered interact.
	// Use on PostLoad (or BeginPlay) (if you've set the interacts on the editor's world outliner
	// unless you've set the reference of the CPuzzle->Interacts on the constructor).
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetLocks(const TArray<bool>& Locks) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetLocks(Locks); }
	}

	// Set the interact pieces to active. Don't call during construction.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetActives(const bool NewEnabled) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetActives(NewEnabled); }
	}

	// sets "DisableWhileAnims" on all the interacts.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetDisableWhileAnims(const bool NewDisabled) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetDisableWhileAnims(NewDisabled); }
	}

	// sets UseHint on this and the linked puzzle items
	virtual void SetUseHint_Implementation(const bool NewHint=true) override {
		Super::SetUseHint_Implementation(NewHint);
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetUseHints(NewHint); }
	}

	// call to reset the puzzle. Override DoReset to do custom logic.
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
	virtual void DoReset_Implementation() {}

	// Interact to *trigger* on Done. It will force unlock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	TObjectPtr<AInteract> DoneInter = nullptr;
	
	// Actor to reward on Done. Can be an interact, it will use fade, but won't trigger.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	TObjectPtr<AActor> DoneActor = nullptr;

	/// subobjects
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCPuzzle> CPuzzle = nullptr;
	
	/// transient

	FTimerHandle ResetTimer;
};
