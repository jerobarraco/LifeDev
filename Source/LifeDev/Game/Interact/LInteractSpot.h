#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LInteractSpot.generated.h"

// A Place to drop other interacts.
// It's always locked, and unlocks only once when all the clouts are dropped.
// It only allows for triggering once it's done.
// It will trigger LockedDlg (when already done) and TriggerDlg (once upon done) accordingly.
// if attempt to use an item once it's done it will trigger FullDlg

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractSpot: public ALInteract {
public:
	GENERATED_BODY()

	ALInteractSpot();

	// When a drop is correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropDlg = NAME_None;

	// When a drop is not correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropBadDlg = NAME_None;

	// When i TRY to drop but it's full
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName DropFullDlg = NAME_None;

	// items to listen to. these are the items that are allowed to be dropped here.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> Items;

protected:
	virtual bool TryTrigger_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	virtual void SetText_Implementation() override;
};
