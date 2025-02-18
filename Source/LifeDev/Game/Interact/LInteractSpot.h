#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LInteractSpot.generated.h"

// A Place to drop other interacts.
// It's always locked, and unlocks only once when all the items are dropped (used with).
// It only allows for triggering once it's done.
// It will trigger LockedDlg (when already done) and TriggerDlg (once upon done) accordingly.
// if attempt to use an item once it's done it will trigger FullDlg
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractSpot: public ALInteract {
	GENERATED_BODY()

public:
	ALInteractSpot();

	// When a drop is correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropDlg = NAME_None;

	// When a drop is not correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropBadDlg = NAME_None;
	// When a drop order is not correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropBadOrderDlg = NAME_None;

	// When you TRY to drop but it's full
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropFullDlg = NAME_None;

	// When you TRY to trigger but it's full.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName LockedFullDlg = NAME_None;

	// items to listen to. these are the items that are allowed to be dropped here.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> Items;

	// when true, the items can only be dropped in order.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseOrder = false;

protected:
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	// virtual void SetText_Implementation() override;
};
