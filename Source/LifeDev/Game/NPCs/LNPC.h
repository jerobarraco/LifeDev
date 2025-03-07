// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LNPC.generated.h"

class UMaterialInterface;

// base npc class. this is not being used. remove?
UCLASS(Blueprintable, BlueprintType)
class ALNPC : public ALInteract {
	GENERATED_BODY()

public:
	ALNPC();
};
