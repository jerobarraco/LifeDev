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

	// Sets which interacts to listen to, and binds. don't call on constructor. call after begin play
	UFUNCTION(BlueprintCallable)
	void SetInteracts(const TArray<AInteract*>& Inters);

	UFUNCTION(BlueprintCallable)
	void Unbind();
	UFUNCTION(BlueprintCallable)
	void Bind();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	EPuzzleType Type = EPuzzleType::SEQUENCE;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TArray<int32> SequenceIDs;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool DisableOnDone = false;
	
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnUpdate OnUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnDone OnDone;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//internal. will modify the sequence toggling the id
	bool CheckSequence(int32 ID);
	bool CheckCombination(int32 ID);
	// internal. to be called when don
	void Done(bool Ok = true) const;

	UFUNCTION() //bound
	void InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj);

	// Set this before begin play to auto bind. Or call SetInteract otherwise. Can be set in the outliner (editor instance).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	TArray<AInteract*> Interacts;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> CurrentIds;
};
