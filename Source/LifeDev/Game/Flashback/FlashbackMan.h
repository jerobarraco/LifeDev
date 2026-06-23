// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once
#include "GameFramework/Info.h"

#include "FlashbackMan.generated.h"

class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

// base class for the flashback manager
UCLASS(Blueprintable)
class LIFEDEV_API AFlashbackMan : public AInfo {
	GENERATED_BODY()

public:
	AFlashbackMan();

	UFUNCTION()
	void ValChanged(const float Value);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called by LGGameMode
	virtual void Init();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UMaterialParameterCollection> MPC = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCInst = nullptr;
};

