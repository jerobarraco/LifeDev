// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CInteract.h"
#include "DelegateWrappers.h"

#include "CPuzzle.generated.h"

class UCInteract;
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

	// don't call on constructor. call after begin play
	UFUNCTION(BlueprintCallable)
	void SetInteracts(const TArray<UCInteract*>& Inters);
	
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnUpdate OnUpdate;
	
	UPROPERTY(BlueprintAssignable, Category="SetUp")
	FPuzzleOnDone OnDone;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	EPuzzleType Type = EPuzzleType::SEQUENCE;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TArray<int32> SequenceIDs;
	
	// TODO implement
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool DisableOnDone = false;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Unbind();
	void Bind();
	
	// will modify the sequence toggling the id
	bool CheckSequence(int32 ID);
	bool CheckCombination(int32 ID);

	// to be called when doned
	void Done(bool Ok = true) const;

	UFUNCTION() //bound
	void InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj);

	// will try to bind if set before begin play
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TArray<UCInteract*> Interacts;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> CurrentIds;
};
