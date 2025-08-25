// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
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
	// Solution describes the order in which the interactions need to be interacted
	// Index 0 is first interact. Solution[0] == the interact that need to be interacted first
	// TODo corroborate that is correct
	SEQUENCE,
	// Solution describes the state in which the interactions needs to be
	// Index 0 is interact 0, Solution[0] == state for interact 0
	COMBINATION,
	// Uses a condition
	CONDITION,
	COUNT UMETA(hidden)
};

// base object to create puzzle like interactions
// it works directly with Interact actors (or interactAnim)
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCPuzzle: public UActorComponent {
	GENERATED_BODY()

public:
	UCPuzzle();

	// will call Reset on the interacts, and reset the current state here.
	// will set ALL interacts as active.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, CallInEditor, Category="Interact|Puzzle")
	void Reset();
	virtual void Reset_Implementation();
	
	// Sets which interacts to listen to, and binds. don't call on constructor. call after begin play
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle")
	void SetPieces(const TArray<TSoftObjectPtr<AInteract>>& NewPieces);

	// purposely returns a copy, to modify the Interacts (after beginplay) call SetInteracts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle")
	FORCEINLINE TArray<TSoftObjectPtr<AInteract>> GetPieces() { return Pieces; }

#pragma region sets
	UFUNCTION(BlueprintCallable)
	void SetDisableWhileAnims(const bool NewDisable) const;

	// Set the interact pieces to enabled
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetActives(const bool NewActive) const;
	// set the UseAutoActives on the pieces
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SetAutoActives(const bool NewAutoActive) const;

	// sets the states on each registered interact.
	// Call on or after begin play.
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetStates(const TArray<int32>& States);
	
	// sets the states on each registered interact.
	// Use on PostLoad (or BeginPlay) (if you've set the interacts on the editor's world outliner
	// unless you've set the reference of the CPuzzle->Interacts on the constructor).
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SetLocks(const TArray<bool>& Locks);

	// set actors as hidden (or not). in game, not editor.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SetHiddensInGame(const bool NewHidden);

	// Sets Use Hint on the linked puzzles.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Puzzle")
	void SetUseHints(const bool NewHint= true);

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Cond"))
	void SetHintConditions(const FString& Cond);
#pragma endregion

	// unbinds from the interacts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void Unbind();
	// binds to the interacts
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void Bind();

	// reset the current values. called on reset and begin play. used internally.
	UFUNCTION(BlueprintCallable, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	void ResetCurrents();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Interact|Puzzle", meta=(AdvancedDisplay))
	const TArray<int32>& GetCurrents() const { return CurrentIds; }

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
	// Used only for Type=Condition. experimental.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp, AdvancedDisplay)
	FString Condition;

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
	bool CheckCondition() const;
	bool IsCurrentSolution();
	void PreDone(const bool Ok=true) const;
	// internal. to be called when done
	void Done(const bool Ok = true) const;

	// Internal. Called when a interact gets triggered.
	UFUNCTION() //bound
	void InterTrigger(UDelegateWrapper* const Wrapper, const int32 ID, UObject* const Obj);

	// Set this before begin play to auto bind. Or call SetInteract otherwise. Can be set in the outliner (editor instance).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<TSoftObjectPtr<AInteract>> Pieces;

	// this is recycled by both modes. and means different things :)
	// on sequence : it's a seq of the ids used
	// on combo: it's a list of each Interact's state
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<int32> CurrentIds;

	//important so they don't get GCd
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UDelegateWrapper>> Wrappers;
};
