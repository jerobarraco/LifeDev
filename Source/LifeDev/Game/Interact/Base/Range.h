// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Interact/InteractAnim.h"

#include "Range.generated.h"

class USphereComponent;
class UCInteract;
class UCAnimatorMix;
class UCRange;

// Base class for Range actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARange: public AActor {
public:
	GENERATED_BODY()

	ARange();

	// lamest lame thing. to test in editor
	UFUNCTION(BlueprintCallable)
	void Trigger();

	UFUNCTION(BlueprintCallable)
	void SetMaxScale(const float Scale) const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UMaterialInterface> Mat = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void AnimEnd();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> Mesh = nullptr;

	// The animator, by default set up for the mesh material and iroot
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorMix> Anim = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> Collider = nullptr;
};
