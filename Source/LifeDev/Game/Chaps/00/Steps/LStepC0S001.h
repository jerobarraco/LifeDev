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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FVector GhostLocation = FVector(210,-42,-65);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* FakeChar = nullptr;

	// where the player will be positioned to blend the camera and when the chapter ends
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* PlayerPos = nullptr;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DestroyActors();
	
	UFUNCTION()
	void TeleportPlayer();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	AGhosts* Ghosts = nullptr;
};
