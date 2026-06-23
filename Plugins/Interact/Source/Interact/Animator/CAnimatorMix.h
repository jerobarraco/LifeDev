// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CAnimatorTrans.h"

#include "CAnimatorMix.generated.h"

class UMaterialInstanceDynamic;
class UCurveFloat;
class USceneComponent;

// A component to animate dynamic material parameters and transforms
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMix: public UCAnimatorTrans {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatFName = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatVName = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor MatVStart = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor MatVEnd = FLinearColor::White;

	// Slower but nicer on colors. Might be better to set to false on simple vector MatV*.
	// only affects MatV*.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	bool UseHSV = true;

	// the material to be animated. Has to be a dynamic material.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	TObjectPtr<UMaterialInstanceDynamic> Mat = nullptr;

protected:
	virtual void Update_Implementation(const float Alpha) override;
	virtual void DeInit() override;
};
