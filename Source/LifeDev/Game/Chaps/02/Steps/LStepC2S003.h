// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S003.generated.h"

class UCRandomizerFB;
// step after the tape. will finish if all the items are obtained.
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S003 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S003();

protected:
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
	UFUNCTION()
	void AfterDlg();
	
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCRandomizerFB* RndFB = nullptr;
};
