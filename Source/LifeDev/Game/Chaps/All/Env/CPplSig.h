// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "JSig/CSignificance.h"
#include "CPplSig.generated.h"


UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent),
Config=LifeDev, DefaultConfig)
class LIFEDEV_API UCPplSig : public UCSignificance {
	GENERATED_BODY()

public:
	UCPplSig();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	ESigValue CalcPplSig(const FTransform& Viewpoint);

	UPROPERTY(BlueprintReadOnly, Config)
	double WaitTimeMin = 10;
	UPROPERTY(BlueprintReadOnly, Config)
	double WaitTimeMax = 20;

	double WaitTime = 5;
	double BeginTime = -1;
};
