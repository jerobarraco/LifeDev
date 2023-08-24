#pragma once
#include "CAnimator.h"
#include "CAnimatorSound.generated.h"

// Animator for a metasound variable
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorSound: public UCAnimator {
	GENERATED_BODY()

public:
	// the sound component to animate
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	UAudioComponent* Snd = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FName IntName = NAME_None;
	// the alpha is 0-1. this would be the max for an int param.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float IntMin = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float IntMax = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FName FloatName = NAME_None;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float FloatMin = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float FloatMax = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	bool VolUse = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float VolMin = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float VolMax = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	bool PitchUse = false;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float PitchMin = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	float PitchMax = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FName TriggerName = NAME_None;

protected:
	virtual void Update_Implementation(float Alpha) override;
};
