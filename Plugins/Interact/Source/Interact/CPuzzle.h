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
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FPuzzleOnUpdate OnUpdate;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FPuzzleOnDone OnDone;

	// TODO implement
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool LockOnDone = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Unbind();
	void Bind();
	void Done(bool Ok = true) const;
	
	UFUNCTION()
	void InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj);

	// will try to bind if set before beginplay
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TArray<UCInteract*> Interacts;
};
