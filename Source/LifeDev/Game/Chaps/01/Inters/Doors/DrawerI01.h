// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Cabinet/Drawer02.h"

#include "DrawerI01.generated.h"

// Drawer 02 instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawerI01: public ADrawer02 {
	GENERATED_BODY()

protected:
	virtual void SetState_Implementation(int32 NewState) override;
};
