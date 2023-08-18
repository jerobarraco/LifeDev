#pragma once
#include "CAnimatorMix.h"
#include "CAnimatorFade.generated.h"

// Animator for the current camera
// This only works with cameras that are not being moved while this is animating
// it will take the current camera position at the begin of playing
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorFade: public UCAnimatorMix {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<UStaticMeshComponent*> Meshes; 

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UMaterialInterface* MatBase; 

	UCAnimatorFade();
protected:
	virtual void BeginPlay() override;
	
	virtual void Begin_Implementation() override;
	virtual void Update_Implementation(float Alpha) override;

};
