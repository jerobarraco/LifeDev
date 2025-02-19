// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S004.generated.h"

// step after the tape has been played.
// will finish if all the items are obtained.
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S004 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S004();

	virtual void TryStart_Implementation() override;
};
