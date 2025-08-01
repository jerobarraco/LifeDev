// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Cabinet/Drawer01.h"

#include "DrawerI00.generated.h"

// warning. this is used in multiple places.
// at least ch00 besides the bed and ch01

// Drawer 01 instance 00. This class is unnecessary. TODO change the drawers in the world to the base and delete
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawerI00: public ADrawer01 {
	GENERATED_BODY()
};
