// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimatorMix.h"
#include "CAnimatorFade.generated.h"

// Animator for the current camera
// This only works with cameras that are not being moved while this is animating
// it will take the current camera position at the begin of playing
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorFade: public UCAnimatorMix {
	GENERATED_BODY()

public:
	UCAnimatorFade();

	// only call in the constructor please.
	UFUNCTION(BlueprintCallable, Category="Interact|AnimFade")
	void SetNewMat();

	// creates the dynamic material. using MatBase, or the material of the 1st mesh in Meshes.
	UFUNCTION(BlueprintCallable, Category="Interact|AnimFade", meta=(UnsafeDuringActorConstruction))
	void CreateMaterial();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TArray<UStaticMeshComponent*> Meshes; 

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp|Material")
	UMaterialInterface* MatBase = nullptr; 
};
