// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S002.generated.h"

class UNiagaraComponent;

// when the chap 1 boss enters
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S002();
	virtual void Start_Implementation() override;

protected:
	// ufunctions needed for bindings

	void SpawnGhosts() const;
	UFUNCTION()
	void StartShake();
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
		FVector(47.491408,906.723833,9.062187));
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UNiagaraComponent* Ghosts = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient)
	AActor* Char = nullptr;
};
