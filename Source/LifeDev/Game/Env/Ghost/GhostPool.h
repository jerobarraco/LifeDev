// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GhostPool.generated.h"


class UPooler;
class UCRandomizer;

// Ghost Pooler
UCLASS(Blueprintable, BlueprintType, Config=LifeDev)
class AGhostPool : public AActor {
	GENERATED_BODY()

public:
	AGhostPool();

	UFUNCTION(BlueprintCallable)
	void SetActive(const bool Act) const;

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

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float FBMin=.4;

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float TrimTime=60;

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float PoolSizeMin=10;

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float PoolSizeMax=20;

	// foxified from sizeMax and min
	UPROPERTY(BlueprintReadOnly, Transient)
	float PoolSize=15;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCRandomizer> Rnd = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UPooler> Pooler = nullptr;
};
