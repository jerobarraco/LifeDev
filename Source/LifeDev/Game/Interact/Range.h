// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Interact/InteractAnim.h"

#include "Range.generated.h"

class UCInteract;
class UCAnimatorTrans;
class UCRange;

// TODO change parent to InteractAnim or LInteract. and remove the usage of the component.
// Base class for Range actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARange: public AInteractAnim {
public:
	GENERATED_BODY()

	ARange();

	// lamest lame thing. to test in editor
	UFUNCTION(BlueprintCallable)
	void ETrigger() { Trigger(); }

	UFUNCTION(BlueprintCallable)
	void SetMaxSize(float Size);

	virtual void Trigger_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	UMaterialInterface* Mat = nullptr;

protected:
	virtual void BeginPlay() override;
	
	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* AnimRoot = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
};
