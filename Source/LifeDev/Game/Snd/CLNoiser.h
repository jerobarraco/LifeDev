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

	UPROPERTY(BlueprintReadWrite)
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION() // bind
	void SetFB(float Value);
};
