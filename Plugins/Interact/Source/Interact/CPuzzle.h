// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CPuzzle.generated.h"

class AInteract;
class UDelegateWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPuzzleOnDone, const bool, IsOk);

UENUM(BlueprintType, Category="Interact|Puzzle")
enum class EPuzzleType: uint8 {
	NONE,
	SEQUENCE,
	COMBINATION,
	COUNT UMETA(hidden)
};

// base object to create puzzle like interactions
// it works directly with Interact actors (or interactAnim)
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCPuzzle: public UActorComponent {
	GENERATED_BODY()

public:
	UCPuzzle();

	// will call Reset on the interacts, and reset the current state here
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, CallInEditor, Category="Interact|Puzzle")
	void Reset();
	virtual void Reset_Implementation();
	
	// Sets which interacts to listen to, and binds. don't call on constructor. call after begin play
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle")
	void SetInteracts(const TArray<AInteract*>& Inters);

	// purposely returns a copy, to modify the Interacts (after beginplay) call SetInteracts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle")
	FORCEINLINE TArray<AInteract*> GetInteracts() { return Interacts; }

	UFUNCTION(BlueprintCallable)
	void SetDisableWhileAnims(const bool NewDisable);
	
	// Set the interact pieces to enabled
	UFUNCTION(BlueprintCallable)
	void SetAutoActives(const bool NewActive);

	// Set the interact pieces to enabled
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetActives(const bool NewActive);

	// sets the states on each registered interact.
	// Call on or after begin play.
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetStates(const TArray<int32>& States);
	
	// sets the states on each registered interact.
	// Use on PostLoad (or BeginPlay) (if you've set the interacts on the editor's world outliner
	// unless you've set the reference of the CPuzzle->Interacts on the constructor).
	UFUNCTION(BlueprintCallable)
	void SetLocks(const TArray<bool>& Locks);
	
	// unbinds from the interacts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void Unbind();
	// binds to the interacts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void Bind();

	// reset the current values. called on reset and begin play. used internally.
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void ResetCurrents();

	// the puzzle type. controls the behavior.
	// beware of changing this after BeginPlay.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	EPuzzleType Type = EPuzzleType::SEQUENCE;

	// The solution for this puzzle.
	// For a Sequence this is a sequence of *Ids* of interactions. in the order you expect them to be triggered.
	// the Id is the *index* in the list of interacts.
	// For a Combination this is a list of the *States index* of each interaction.
	// This array must have the same length as the Interacts
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<int32> Solution;

	// if true then the Interact will disable once toggled.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Disable")
	bool DisableOnInter = false;

	// will disable the interacts when the puzzle is done.
	// Beware of unchecking "DisableWhileAnim" on the interacts or this won't work well.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Disable")
	bool DisableOnDone = false;

	// triggers when the puzzle is updated. (i.e. when an interaction is triggered).
	UPROPERTY(BlueprintAssignable, Category="SetUp|Events")
	FPuzzleOnUpdate OnUpdate;

	// triggers on reset
	UPROPERTY(BlueprintAssignable, Category="SetUp|Events")
	FPuzzleOnReset OnReset;

	// triggers when the puzzle is completed, with or without success.
	// after anim.
	UPROPERTY(BlueprintAssignable, Category="SetUp|Events")
	FPuzzleOnDone OnDone;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//internal. will modify the sequence toggling the id
	bool CheckSequence(const int32 ID);
	bool CheckCombination(const int32 ID);
	bool IsCurrentSolution();
	// internal. to be called when done
	void Done(const bool Ok = true) const;

	// Internal. Called when a interact gets triggered. 
	UFUNCTION() //bound
	void InterTrigger(UDelegateWrapper* const Wrapper, const int32 ID, UObject* const Obj);

	// Set this before begin play to auto bind. Or call SetInteract otherwise. Can be set in the outliner (editor instance).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<TObjectPtr<AInteract>> Interacts;

	// this is recycled by both modes. and means different things :)
	// on sequence : it's a seq of the ids used
	// on combo: it's a list of each Interact's state
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<int32> CurrentIds;

	//important so they don't get GCd
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UDelegateWrapper>> Wrappers;
};
