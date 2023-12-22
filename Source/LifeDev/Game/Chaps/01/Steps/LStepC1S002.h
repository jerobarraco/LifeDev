// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S002.generated.h"

class AGhosts;

// when the chap 1 boss enters
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALStepC1S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S002();
	virtual void Start_Implementation() override;

protected:
	// ufunctions are needed for bindings

	void SpawnGhosts();
	UFUNCTION()
	void StartShake();
	UFUNCTION()
	void StopShake();
	UFUNCTION()
	void ShakeStarted();
	UFUNCTION()
	void GhostDestroyed();
	UFUNCTION() // bind
	void DestroyGhosts();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UClass* ShakeClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<AActor> CharClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FTransform CharT = FTransform(
		FRotator(0.000000,-109.999999,0.000000),
		FVector(-102.567499,918.952130,11.503022));

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FVector GhostLocation = FVector(200,-4,-75);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AGhosts* Ghosts = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AActor* Char = nullptr;
};
