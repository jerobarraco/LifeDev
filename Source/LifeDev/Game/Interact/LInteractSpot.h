#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LInteractSpot.generated.h"

// A Place to drop other interacts.
// It's always locked, and triggers only once when all the items are dropped (used with).
// It will trigger LockedDlg (when not done) and TriggerDlg (once upon done) accordingly.
// disabled: if attempt to use an item once it's done it will trigger FullDlg
// always has two states: enabled, done.
// by default is disabled after completed.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractSpot: public ALInteract {
	GENERATED_BODY()

public:
	ALInteractSpot();

	// items to receive/consume. these are the items that are allowed to be dropped here.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> Items;

	// when true, the items can only be dropped in order.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseOrder = false;

	// when true it will increase the state on each usage
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseStateInc = false;

protected:
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	virtual bool TryTrigger_Implementation() override;
};
