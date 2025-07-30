#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LSpot.generated.h"

// A Place to drop other interacts.
// It's always locked, and triggers only once when all the items are dropped (used with).
// It will trigger LockedDlg (when not done) and TriggerDlg (once upon done) accordingly.
// disabled: if attempt to use an item once it's done it will trigger FullDlg
// always has two states: enabled, done.
// by default is disabled after completed.
// this is meant to only define base attributes, not to extend functionality from linteract
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALSpot: public ALInteract {
	GENERATED_BODY()

public:
	ALSpot();
};
