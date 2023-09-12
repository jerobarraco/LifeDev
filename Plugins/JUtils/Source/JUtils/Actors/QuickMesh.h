// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "QuickMesh.generated.h"

class UCQuickMesh;
// base class for an optimized mesh. by default it well have EVERYTHING disabled, mostly.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AQuickMesh: public AActor {
	GENERATED_BODY()

public:
	AQuickMesh();

	UPROPERTY(BlueprintReadWrite)
	UCQuickMesh* Mesh = nullptr;
};
