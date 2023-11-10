// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Card.h"

#include "Card00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ACard00: public ACard {
	GENERATED_BODY()

public:
	ACard00();
	
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void PostActorCreated() override;
	virtual void BeginPlay() override;
};
