// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC01.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALNPC01 : public ALNPCH {
	GENERATED_BODY()

public:
	ALNPC01();
	
protected:
	UFUNCTION()
	void DiagDone();
	void TriggerLocked_Implementation();
};
