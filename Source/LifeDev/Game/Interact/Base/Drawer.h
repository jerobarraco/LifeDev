// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Doors/Doorish.h"

#include "Drawer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawer: public ADoorish {
	GENERATED_BODY()

public:
	ADrawer();
	virtual void BeginPlay() override;
};
