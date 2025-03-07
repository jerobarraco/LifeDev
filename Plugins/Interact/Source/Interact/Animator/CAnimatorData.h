// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CAnimatorTrans.h"

#include "CAnimatorData.generated.h"

class UCurveFloat;
class USceneComponent;

// while this is a cool idea, i'm not going to use it in my game.
// because of how i've already organized the assets.
// Also using custom primitive data on materials is a mayor pain. so it's not practical.
// Though i love performance, i'm not into masochism.
// And lastly and most importantly, Nanite seems to be grouping the assets on their material even though they are dynamic.
// Which means the performance gain is small. Unless i've got that wrong.

// Allows to animate custom data on primitives (meshes)
// mostly for materials that are not dynamic.
// Could perform better than dynamic materials if you have many actors using the same material.
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact),
	meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorData: public UCAnimator {
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

	// Slower but nicer on colors.
	// Might be better to set to false on simple vector DataV*.
	// only affects DataV*.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	bool UseHSV = true;

	// primitive component to be animated (only for data.)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	TObjectPtr<UPrimitiveComponent> Prim = nullptr;

protected:
	virtual void Update_Implementation(const float Alpha) override;
	virtual void DeInit() override;
};
