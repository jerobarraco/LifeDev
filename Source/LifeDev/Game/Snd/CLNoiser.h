// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only


#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

#include "Sounds/CNoiser.h"

#include "CLNoiser.generated.h"

// Generates random noises. interacts with the flashback system.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLNoiser : public UCNoiser {
	GENERATED_BODY()

public:
	UCLNoiser();

	virtual void Activate(bool bReset = false) override;
	
	// maximum distance with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float DistFBMax = 120.0;
	// minimum distance with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float DistFBMin = 60.0;

	// maximum time with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFBMax = 20.0;
	// minimum time with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFBMin = 10.0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION() // bind
	void FeatUpdate(EFeat Feat, bool bEnabled);
	UFUNCTION() // bind
	void SetFB(float Value);
};
