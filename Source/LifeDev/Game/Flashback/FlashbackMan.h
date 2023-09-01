// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "FlashbackMan.generated.h"

class UMaterialParameterCollectionInstance;

// base class for the flashback manager
UCLASS(Blueprintable, config=Game)
class LIFEDEV_API AFlashbackMan : public AActor {
	GENERATED_BODY()

public:
	
	AFlashbackMan();

	UFUNCTION()
	void ValChanged(float Value);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UMaterialParameterCollection* MPC = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UMaterialParameterCollectionInstance* MPCInst = nullptr;
};