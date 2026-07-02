// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "GameFramework/Actor.h"

#include "QuickMesh.generated.h"

class UCQuickMesh;

// base class for an optimized mesh. by default, it will have EVERYTHING disabled, mostly.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils))
class JUTILS_API AQuickMesh: public AActor {
	GENERATED_BODY()

public:
	AQuickMesh();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UCQuickMesh> Mesh = nullptr;
};
