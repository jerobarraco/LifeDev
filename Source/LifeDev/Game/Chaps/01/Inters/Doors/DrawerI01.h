// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Cabinet/Drawer01.h"

#include "DrawerI01.generated.h"

// Drawer 02 instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADrawerI01: public ADrawer01 {
	GENERATED_BODY()

protected:
	virtual void SetState_Implementation(const int32 NewState) override;
};
