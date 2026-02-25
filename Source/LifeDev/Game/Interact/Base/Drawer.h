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
	
	// transitional property to allow drawers to have relative tends with arbitrary ROTATIONS (scale not taken into account)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(Deprecated), Category="SetUp|Anim")
	bool IsTEndRelative = false;
};
