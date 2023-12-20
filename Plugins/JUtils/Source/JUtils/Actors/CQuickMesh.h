// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

// TODO move to Actors
#include "CQuickMesh.generated.h"

// base class for an optimized mesh. by default it well have EVERYTHING disabled, mostly.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API UCQuickMesh: public UStaticMeshComponent {
	GENERATED_BODY()

public:
	void SetQuickCollisionEnabled(bool Enable);
	UCQuickMesh();

	UFUNCTION(BlueprintCallable)
	void SetCastAllShadows(bool Cast);
};
