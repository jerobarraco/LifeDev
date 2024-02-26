// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "CPuzzle.generated.h"

class AInteract;
class UDelegateWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPuzzleOnDone, bool, IsOk);

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
	void SetDisableWhileAnims(bool NewDisable);
	
	// Set the interact pieces to enabled
	UFUNCTION(BlueprintCallable)
	void SetEnableds(bool NewEnabled);
		
	// sets the states on each registered interact.
	// Call on or after begin play.
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable)
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	EPuzzleType Type = EPuzzleType::SEQUENCE;
	
	// The solution for this puzzle.
	// For a Sequence this is a sequence of *Ids* of interactions. in the order you expect them to be triggered.
	// the Id is the *index* in the list of interacts.
	// For a Combination this is a list of the *States index* of each interaction.
	// This array must have the same length as the Interacts
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<int32> Solution;

	// if true then the interact will disable once toggled.
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
	UPROPERTY(BlueprintAssignable, Category="SetUp|Events")
	FPuzzleOnDone OnDone;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//internal. will modify the sequence toggling the id
	bool CheckSequence(int32 ID);
	bool CheckCombination(int32 ID);
	bool IsCurrentSolution();
	// internal. to be called when done
	void Done(bool Ok = true) const;

	// Internal. Called when a interact gets triggered. 
	UFUNCTION() //bound
	void InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj);

	// Set this before begin play to auto bind. Or call SetInteract otherwise. Can be set in the outliner (editor instance).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<AInteract*> Interacts;

	// this is recycled by both modes. and means different things :)
	// on sequence : its a seq of the ids used
	// on combo: its a list of each interact's state
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<int32> CurrentIds;

	//important so they don't get GCd
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<UDelegateWrapper*> Wrappers;
};
