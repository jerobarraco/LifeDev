// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "CPuzzle.generated.h"

class AInteract;
class UDelegateWrapper;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPuzzleOnDone, bool, IsOn);

UENUM(BlueprintType)
enum class EPuzzleType: uint8 {
	NONE,
	SEQUENCE,
	COMBINATION,
	COUNT UMETA(hidden)
};

// base object to create puzzle like interactions
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCPuzzle: public UActorComponent {
	GENERATED_BODY()

public:
	UCPuzzle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, CallInEditor)
	void Reset();
	virtual void Reset_Implementation();
	
	// Sets which interacts to listen to, and binds. don't call on constructor. call after begin play
	UFUNCTION(BlueprintCallable)
	void SetInteracts(const TArray<AInteract*>& Inters);
	UFUNCTION(BlueprintCallable)
	void Unbind();
	UFUNCTION(BlueprintCallable)
	void Bind();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	EPuzzleType Type = EPuzzleType::SEQUENCE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<int32> SolutionIDs;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool DisableOnDone = false;
	
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnUpdate OnUpdate;

	// triggers when the puzzle is completed, with or without success.
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnDone OnDone;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//internal. will modify the sequence toggling the id
	bool CheckSequence(int32 ID);
	bool CheckCombination(int32 ID);
	bool IsCurrentSolution();
	// internal. to be called when don
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
};
