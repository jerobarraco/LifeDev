// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S003.generated.h"

class UNiagaraComponent;
// blending to and spawning npc01
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S003 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S003();

	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
	
protected:
	// ufunctions needed for bindings
	UFUNCTION()
	void NPCDiagStop();
	UFUNCTION()
	void NPCDiagStart();
	UFUNCTION()
	void NPCSpawn();
	void NPCDestroy();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<AActor> CharClass = nullptr;
	// TODO fix this position is not working!!!!
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FTransform CharT = FTransform(
		FRotator(0.000000,-289.999998,0.000000),
		FVector(-140.801513,899.912999,10.516852));
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient)
	AActor* Char = nullptr;
};
