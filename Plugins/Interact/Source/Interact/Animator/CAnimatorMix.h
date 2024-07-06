// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimatorTrans.h"

#include "CAnimatorMix.generated.h"

class UCurveFloat;
class USceneComponent;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMix: public UCAnimatorTrans {
	GENERATED_BODY()

public:
	// Mats
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatFName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	int32 MatFIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatVName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	int32 MatVIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor MatVStart = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor MatVEnd = FLinearColor::White;

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	UMaterialInstanceDynamic* Mat = nullptr;

	// primitive component to be animated (only for data.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	UPrimitiveComponent* Prim = nullptr;
	
	// ~Mats

protected:
	virtual void Update_Implementation(float Alpha) override;
	virtual void DeInit() override;
};
