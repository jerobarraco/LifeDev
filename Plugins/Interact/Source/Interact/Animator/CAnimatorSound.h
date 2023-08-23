#pragma once
#include "CAnimator.h"
#include "CAnimatorSound.generated.h"

// Animator for a metasound variable
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorSound: public UCAnimator {
	GENERATED_BODY()

public:
	// the sound component to animate
	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	UAudioComponent* Snd = nullptr;

	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	FName IntName = NAME_None;
	// the alpha is 0-1. this would be the max for an int param.
	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	float IntMin = 0;
	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	float IntMax = 10;

	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	FName FloatName = NAME_None;
	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	float FloatMin = 0;
	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	float FloatMax = 1;

	UPROPERTY(BlueprintCallable, EditDefaultsOnly, Category="SetUp")
	FName TriggerName = NAME_None;
	
protected:
	virtual void Update_Implementation(float Alpha) override;
};
