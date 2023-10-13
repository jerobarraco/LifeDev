// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC0S000.generated.h"

class UCRandomizer;
class AActor;

UCLASS(Blueprintable, BlueprintType)
class ALStepC0S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC0S000();
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;

protected:
	virtual void Debug_Implementation() override;
	void ShowDoors(bool Chap00);

	// the door of the toilet on chap0
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* C0Door = nullptr;

	// the door of the toilet on chap1
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* C1Door = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	UCRandomizer* RandFB = nullptr;
};
