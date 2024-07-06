// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimatorTrans.h"

#include "CAnimatorData.generated.h"

class UCurveFloat;
class USceneComponent;

// while this is a cool idea, i'm not going to use it.
// because of how i've already organized the assets.
// Nanite seems to be grouping the assets on their material even though they are dynamic.

// Allows to animate custom data on primitives (meshes)
// only for the non-dynamic materials though.
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorData: public UCAnimatorTrans {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	int32 DataFIndex = INDEX_NONE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float DataFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float DataFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	int32 DataVIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor DataVStart = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FLinearColor DataVEnd = FLinearColor::White;

	// primitive component to be animated (only for data.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	UPrimitiveComponent* Prim = nullptr;

protected:
	virtual void Update_Implementation(float Alpha) override;
	virtual void DeInit() override;
};
