// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GhostPool.generated.h"


class UPooler;
class UCRandomizer;
// Ghost Pooler
UCLASS(Blueprintable, BlueprintType)
class AGhostPool : public AActor {
	GENERATED_BODY()

public:
	AGhostPool();

	void SetActive(bool Act);

	// will deactivate and attempt to kill active ghosts
	// All: when set will try to kill al the ghosts on the pool and empty it.
	UFUNCTION(BlueprintCallable)
	void Kill(const bool All=false);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void FBTo(const float To);

	UFUNCTION()
	void Spawn();

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass = AActor::StaticClass();
	// avoid include on header.

	UPROPERTY(BlueprintReadWrite)
	float FBMin=.4;

	UPROPERTY(BlueprintReadWrite)
	float PoolMax=15;

	UPROPERTY(BlueprintReadWrite)
	float TrimTime=60;

	UPROPERTY(BlueprintReadOnly)
	UCRandomizer* Rnd = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UPooler* Pooler = nullptr;
};
