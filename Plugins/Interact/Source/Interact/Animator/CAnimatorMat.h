// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "CAnimator.h"

#include "CAnimatorMat.generated.h"

class USceneComponent;

// An interactive actor that can have an animation. Use UCAnimatorMix
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent, DeprecatedNode))
class INTERACT_API UCAnimatorMat: public UCAnimator {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName ParamFName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName ParamVName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FVector VStart;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FVector VEnd = FVector::OneVector;

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UMaterialInstanceDynamic* Mat = nullptr;

protected:
	virtual void Update_Implementation(float Alpha) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
