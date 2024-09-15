// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "TestScreen.generated.h"


class UPooler;
class UCRandomizer;
// Ghost Pooler
UCLASS(Blueprintable, BlueprintType)
class ATestScreen : public AActor {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void Captured(int32 Width, int32 Height, const TArray<FColor>& Colors);
};
