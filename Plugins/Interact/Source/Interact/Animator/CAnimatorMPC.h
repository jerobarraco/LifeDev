// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CAnimatorTrans.h"

#include "CAnimatorMPC.generated.h"

class UCurveFloat;
class USceneComponent;

// A component to animate parameters on a material parameter collection
UCLASS(Blueprintable, BlueprintType, Placeable,
	ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMPC: public UCAnimator {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName ParFName = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float ParFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float ParFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName ParVName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor ParVStart = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor ParVEnd = FLinearColor::White;

	// Slower but nicer on colors. Might be better to set to false on simple vector ParV*.
	// only affects ParV*.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	bool UseHSV = true;

	// the (Material Parameter) Collection to animate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	TObjectPtr<UMaterialParameterCollection> MPC = nullptr;

protected:
	virtual void Update_Implementation(const float Alpha) override;
	virtual void DeInit() override;
	virtual void Begin_Implementation() override;
	virtual void End_Implementation() override;
	
	// the instance to animate
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCI = nullptr;
};
