// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "CAnimatorTrans.h"

#include "CAnimatorMix.generated.h"

class UCurveFloat;
class USceneComponent;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMix: public UCAnimatorTrans {
	GENERATED_BODY()

public:

	// Mats
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatFName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatVName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FVector MatVStart = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FVector MatVEnd = FVector::OneVector;

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	UMaterialInstanceDynamic* Mat = nullptr;
	// ~Mats
protected:
	virtual void Update_Implementation(float Alpha) override;
	virtual void DeInit() override;

	virtual void BeginPlay() override;
};
