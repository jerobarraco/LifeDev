// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LPuzzle.generated.h"

// Base class for LPuzzles
// unfortunately unreal does not allow to have multiple inheritance
// so this reimplements APuzzle and does not inherit it.
// When using hints, use it through this class. (e.g. set UseHint and HintCondition here)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALPuzzle: public ALInteract {
	GENERATED_BODY()

public:
#pragma region base
	ALPuzzle();

	virtual void SetActorHiddenInGame(bool NewHidden) override;
	virtual bool ShowHint_Implementation() override;
	// call to reset the puzzle. Override DoReset to do custom logic.
	virtual void Reset() override;
#pragma endregion

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
	FORCEINLINE void SetActives(const bool NewActive) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetActives(NewActive); }
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetAutoActives(const bool NewActive) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetAutoActives(NewActive); }
	}

	// sets "DisableWhileAnims" on all the interacts.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetDisableWhileAnims(const bool NewDisabled) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetDisableWhileAnims(NewDisabled); }
	}

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

	// called when the puzzle is done. either on fail or success.
	// override Trigger and TriggerLocked is preferred
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Done(const bool Ok);

	// called when the puzzle is updated. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Update();
	
	// called when the puzzle is reset. override if needed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DoReset();
	virtual void DoReset_Implementation() {};

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCPuzzle> CPuzzle = nullptr;
	
	/// transient

	FTimerHandle ResetTimer;
};
