// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LPuzzle.generated.h"

// Base class for LPuzzles
// unreal does not allow to have multiple inheritance so this is the best way to implement it, and can't be on a plugin.
// When using hints, use it through this class. (e.g. set UseHint and HintCondition here)
// avoid using postLoad to set attributes. that will execute only on map load, not when the game runs (on pie),
// and the changes are not serialized properly (it's between being serialized and not, and is saved only when other things are modified)
// so it creates a ton of false positives.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALPuzzle: public ALInteract {
	GENERATED_BODY()

public:
#pragma region base
	ALPuzzle();

	virtual void SetActorHiddenInGame(bool NewHidden) override;
	// call to reset the puzzle. Override DoReset to do custom logic.
	virtual void Reset() override;
	virtual void SetActive_Implementation(const bool Active = true) override;
	virtual void SetUseHint_Implementation(const bool NewUseHint = true) override;
	virtual void Fade_Implementation(const bool FadeIn = false, const bool SetHidden = true) override;
#pragma endregion
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetHiddensInGame(const bool IsHidden) {
		SetActorHiddenInGame(true);
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetHiddensInGame(IsHidden); }
	}

	// sets the states on each registered interact.
	// Call on, or after, begin play (but not before).
	// Note that this will reset the cpuzzle (and interacts) 
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetStates(const TArray<int32>& States) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetStates(States); }
	}
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetStatesNow(const TArray<int32>& States) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetStatesNow(States); }
	}

	// sets the states on each registered interact.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetLocks(const TArray<bool>& Locks) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetLocks(Locks); }
	}

	// Set the interact pieces to active. Don't call during construction.
	// Calling during BeginPlay is risky, as the pieces might have not gone through their BeginPlay,
	// and if they have AutoActive set, then they might activate again.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetActives(const bool NewActive) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetActives(NewActive); }
	}

	// note, calling this on begin play might not be the best,
	// as the puzzle pieces might have gone through their BeginPlay.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(UnsafeDuringActorConstruction))
	FORCEINLINE void SetAutoActives(const bool NewActive) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetAutoActives(NewActive); }
	}

	// sets "DisableWhileAnims" on all the interacts.
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	FORCEINLINE void SetDisableWhileAnims(const bool NewDisabled) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetDisableWhileAnims(NewDisabled); }
	}

	// sets UseHint on this and the linked puzzle items
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void SetUseHints(const bool NewHint=true) {
		UseHint = NewHint;
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetUseHints(NewHint); }
	}
	
	// set hint conditions
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Cond"))
	void SetHintConditions(const FString& Cond) const {
		if (LIKELY(IsValid(CPuzzle))) { CPuzzle->SetHintConditions(Cond); }
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

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCPuzzle> CPuzzle = nullptr;
	
	/// transient

	FTimerHandle ResetTimer;
};
