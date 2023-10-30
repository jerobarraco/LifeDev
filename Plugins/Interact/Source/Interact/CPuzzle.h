// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CInteract.h"

#include "CPuzzle.generated.h"

class UCInteract;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleOnUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPuzzleOnDone, bool, IsOn);

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

	
	// TODO make private, add function to set, unbind then bind on set
	// list of interacts to bind to
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Unbind();
	void Bind();
	void Done(bool Ok = true) const;
	
	UFUNCTION() // bind
	void Triggered();

	// will try to bind if set before beginplay
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TArray<UCInteract*> Interacts;
};
