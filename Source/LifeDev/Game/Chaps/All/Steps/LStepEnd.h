// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepEnd.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALStepEnd : public ALStep {
	GENERATED_BODY()
	
public:
	ALStepEnd();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString NextLevel = "Outro_L";

protected:
	virtual void BeginPlay() override;
	void OpenLevel();
	virtual void TryStart_Implementation() override;
	virtual void Start_Implementation() override;
	

	UFUNCTION(BlueprintCallable, CallInEditor)
	void dont(int64 A);
	static void (ALStep::*fp)(void);
};
