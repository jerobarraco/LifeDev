// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC0S001.generated.h"

class AGhosts;

UCLASS(Blueprintable, BlueprintType)
class ALStepC0S001 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC0S001();
	
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* FakeChar = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FVector GhostLocation = FVector(210,-42,-65);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Transient)
	AGhosts* Ghosts = nullptr;
};
