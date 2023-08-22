// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Cabinet/Drawer01.h"

#include "DrawerI00.generated.h"

// Drawer 01 instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawerI00: public ADrawer01 {
	GENERATED_BODY()
public:

protected:
	void Trigger_Implementation();
};
