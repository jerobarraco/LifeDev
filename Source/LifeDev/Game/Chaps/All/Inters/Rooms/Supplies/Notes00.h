// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "Notes00.generated.h"

class UTextRenderComponent;

UCLASS(Blueprintable)
class LIFEDEV_API ANotes00 : public ALInteract {
	GENERATED_BODY()

public:
	ANotes00();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UTextRenderComponent> Text;
};
