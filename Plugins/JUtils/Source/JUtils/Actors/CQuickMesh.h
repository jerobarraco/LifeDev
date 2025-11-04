// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CQuickMesh.generated.h"

// base class for an optimized mesh. by default, it will have EVERYTHING disabled, mostly.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API UCQuickMesh: public UStaticMeshComponent {
	GENERATED_BODY()

public:
	UCQuickMesh();
	
	UFUNCTION(BlueprintCallable)
	void SetQuickCollisionEnabled(const bool Enable);

	UFUNCTION(BlueprintCallable)
	void SetCastAllShadows(const bool Cast);

	// a bit of optimization. When "Contact" is set it will use contact shadows and disable dynamic shadows.
	// your lights need to have a contact shadow length.
	UFUNCTION(BlueprintCallable)
	void SetCastContactShadow(const bool Contact);

	UFUNCTION(BlueprintCallable)
	void SetCustomDepth(const bool Enable, const int32 Val=0);
};
