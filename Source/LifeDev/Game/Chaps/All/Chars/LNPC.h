// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LNPC.generated.h"

// base npc class
UCLASS(Blueprintable, BlueprintType)
class ALNPC : public AActor {
	GENERATED_BODY()

public:
	ALNPC();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root;
};
