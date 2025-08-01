// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Chaps/All/Inters/Doors/Cabinet/Drawer02.h"

#include "DrawerI01.generated.h"

// warning. this is potentially used in multiple places.
// at least on ch00 besides the bed.

// TODO replace the triggeldlg with auto dialog, then remove this class like draweri00

// Drawer 02 instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawerI01: public ADrawer02 {
	GENERATED_BODY()
};
