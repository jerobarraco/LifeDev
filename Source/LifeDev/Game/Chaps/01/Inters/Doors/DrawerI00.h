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
	virtual void SetState_Implementation(const int32 NewState) override;
};
