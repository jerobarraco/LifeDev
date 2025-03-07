// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CAnimatorMix.h"
#include "CAnimatorFade.generated.h"

// Animator for materials. Deprecated. It's preferred to use AnimatorTrans
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact),
	meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorFade: public UCAnimatorMix {
	// It's a mix because i prefer to have the trans stuff than adding a new duplicated code class for the mat only
	GENERATED_BODY()

public:
	UCAnimatorFade();

	// only call during construction. sets to the voxelfade mat.
	UFUNCTION(BlueprintCallable)
	void SetDefaultMat();
	
	// creates the dynamic material. using MatBase, or the material of the 1st mesh in Meshes.
	UFUNCTION(BlueprintCallable, Category="Interact|AnimFade", meta=(UnsafeDuringActorConstruction))
	void CreateMaterial();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TArray<TObjectPtr<UStaticMeshComponent>> Meshes;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Material")
	TObjectPtr<UMaterialInterface> MatBase = nullptr;
};
