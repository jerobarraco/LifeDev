// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LightPool.generated.h"

enum class EFeat : uint8;
class UPooler;
class UCRandomizer;

// Light Pooler. Controlled by EFeat::E_Lights
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class ALightPool : public AInfo {
	GENERATED_BODY()

public:
	ALightPool();

	UFUNCTION(BlueprintCallable)
	void SetActive(const bool Act) const;

	// will deactivate and attempt to kill active ghosts
	UFUNCTION(BlueprintCallable)
	void Kill(const bool All=false);

	UFUNCTION(BlueprintCallable)
	void Init();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION() // bind
	void FeatUpdate(const EFeat Feat, const bool bEnabled);
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
