// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Sounds/CNoiser.h"

#include "CLNoiser.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLNoiser : public UCNoiser {
	GENERATED_BODY()

public:
	UCLNoiser();

	// maximum distance with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float DistFBMax = 120.0;
	// minimum distance with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float DistFBMin = 50.0;

	// maximum time with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFBMax = 10.0;
	// minimum time with flashback system
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TimeFBMin = 5.0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION() // bind
	void SetFB(float Value);
};
