// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/Base/Lights/LSwitch.h"

#include "LSwitch00.generated.h"

// base for the base for light switches. has a base mesh.
UCLASS(Blueprintable, BlueprintType)
class ALSwitch00 : public ALSwitch {
	GENERATED_BODY()

public:
	ALSwitch00();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Base = nullptr;
};
