// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"
#include "LSettings.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LFeatsMan.generated.h"

class ALGGameMode;

// Handles feat changes and init for things that can't do it themselves ONLY
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALFeatsMan : public AInfo {
	GENERATED_BODY()

public:
	ALFeatsMan();


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called by LGGameMode
	virtual void Init();

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float FringeIntensity = 1;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurMax = 5;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurAmount = .5;

protected:
	void LoadMPC();
	void LoadFeats();
	UFUNCTION()
	void FeatUpVisual(const EFeat Feat, const bool bEnabled);
	UFUNCTION()
	void FeatUpUnreal(const EFeat Feat, const bool bEnabled);
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ALGGameMode> GM = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UMaterialParameterCollection> MPC = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialParameterCollectionInstance> MPCI = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> SpeedMat = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInterface> FBMat = nullptr;
};

