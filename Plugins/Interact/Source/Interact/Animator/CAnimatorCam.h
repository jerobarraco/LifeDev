#pragma once
#include "CAnimator.h"
#include "CAnimatorCam.generated.h"

// Animator for the current camera
// This only works with cameras that are not being moved while this is animating
// it will take the current camera position at the begin of playing
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorCam: public UCAnimator {
	GENERATED_BODY()

public:
	// Target point to look at
	UPROPERTY(BlueprintReadWrite, Category="SetUp")
	FVector Target;
	
protected:
	virtual void Begin_Implementation() override;
	virtual void Update_Implementation(float Alpha) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	APlayerController* Controller = nullptr;
	FRotator CamRotEnd;
	FRotator CamRotStart;
};
