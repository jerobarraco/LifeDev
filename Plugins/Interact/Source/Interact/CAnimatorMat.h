// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "CAnimatorRaw.h"

#include "CAnimatorMat.generated.h"

class USceneComponent;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMat: public UCAnimatorRaw {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName ParamFName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
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
