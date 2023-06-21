#pragma once

#include "CoreMinimal.h"
#include "Dialogs/Dialogs.h"
#include "Interact/InteractAnim.h"
#include "Inventory/Inventory.h"

#include "LInteract.generated.h"
// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteract: public AInteractAnim {
public:
	GENERATED_BODY()

	// name of the item that is needed to "have" to unlock this. (just having it will unlock it, unless we also set ULockItem)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItemReq = NAME_None;
	
	// name of the item that will unlock this. setting it will lock the actor on start.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItem = NAME_None;

	// Dialog to show when unlocking, or none to not say anything
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockDlg = NAME_None;

	// dialog to trigger when tried to use the wrong item to unlock this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockBadDlg = NAME_None;

	// dialog to display if this object is locked AND we have the ULockItem. Not setting it will result in using LockDlg 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItemDlg = NAME_None;

	// dialog to show when the object is locked and we DON'T have the ULockItem
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName LockDlg = NAME_None;

	// dialog to show when the object is triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName TriggerDlg = NAME_None;

	// setting this will reward the item on trigger and self-destruct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName ItemReward = NAME_None;

protected:
	virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;
	virtual bool TryTrigger_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
};
