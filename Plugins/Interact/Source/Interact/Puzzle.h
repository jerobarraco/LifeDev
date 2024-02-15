// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Puzzle.generated.h"

class AInteract;
class UCPuzzle;

// Base class for Puzzles that can optionally trigger an interact
UCLASS(Blueprintable, BlueprintType)
class INTERACT_API APuzzle: public AActor {
	GENERATED_BODY()

public:
	APuzzle();
	
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

	// Set the interact pieces to enabled
	UFUNCTION(BlueprintCallable)
	void SetEnableds(bool NewEnabled);

protected:
	// called when the puzzle is done. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Done(bool IsOk);
	virtual void Done_Implementation(bool IsOk);

	// called when the puzzle is updated. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Update();
	virtual void Update_Implementation() {};
	
	// called when the puzzle is reset. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DoReset();
	virtual void DoReset_Implementation() {};

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Interact to trigger on Done. It will force unlock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|OnDone")
	AInteract* DoneInter = nullptr;

	/// subobjects
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCPuzzle* CPuzzle = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	USceneComponent* Root = nullptr;
};
