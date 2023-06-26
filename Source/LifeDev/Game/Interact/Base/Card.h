// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Card.generated.h"

// Base class for door-like and drawer actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ACard: public ALInteract {
public:
	GENERATED_BODY()

	ACard();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Frame = nullptr;
};
