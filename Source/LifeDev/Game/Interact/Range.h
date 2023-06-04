// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Range.generated.h"

class UCInteract;
class UCAnimatorTrans;
class UCRange;

// Base class for Range actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARange: public AActor {
public:
	GENERATED_BODY()

	ARange();

	// lamest lame thing
	UFUNCTION(BlueprintCallable)
	void ETrigger() { Trigger(); }
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Trigger();
	virtual void Trigger_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetMaxSize(float Size);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	UMaterialInterface* Mat = nullptr;

protected:
	UFUNCTION()
	void AnimUpdate(float Progress, float Alpha);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// the root for animations, and positioning the mesh.
	// Don't change the transform of this guy. change the transform of the children.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* AnimRoot = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorTrans* Animator = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UMaterialInstanceDynamic* MatI = nullptr;
};
