// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once
#include "CoreMinimal.h"
#include "LSysSettings.h"

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

	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float FringeIntensity = 1;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurMax = 5;
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	float MotionBlurAmount = .5;
	
protected:
	UFUNCTION()
	void FeatVisualUpdate(EFeat Feat, bool bEnabled);

	UPROPERTY(BlueprintReadOnly, Transient)
	ALGGameMode* GM = nullptr;
};
