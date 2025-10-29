// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "Ppl.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APpl : public ALInteract {
	GENERATED_BODY()

public:
	APpl();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Head = nullptr;
};
