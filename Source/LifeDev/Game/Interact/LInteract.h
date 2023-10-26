#pragma once

#include "CoreMinimal.h"
#include "Interact/InteractAnim.h"

#include "LInteract.generated.h"

class UFlags;
class UCAnimatorFade;
class UDiags;
class UInventory;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteract: public AInteractAnim {
	GENERATED_BODY()

public:
	ALInteract();
	
	// name of the item that is needed to "have" to unlock this. (just having it will unlock it, unless we also set ULockItem)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItemReq = NAME_None;
		
	// name of the flag that is needed to "have" to unlock this.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockFlagReq = NAME_None;
	
	// name of the item that will unlock this. setting it will lock the actor on start.
	// it will also decide whether to shod LockedDlg or LockedItemDlg on trigger(locked)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItem = NAME_None;

	// Dialog to show when unlocking, or none to not say anything.
	// After this the TriggerDlg will trigger too. But opposed to TriggerDlg this only shows when unlocking.
	// (e.g. useful for doors) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockDlg = NAME_None;

	// dialog to trigger when tried to use the wrong item to unlock this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockBadDlg = NAME_None;

	// dialog to display if this object is locked AND we have the ULockItem. Not setting it will result in using LockDlg 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName LockedItemDlg = NAME_None;

	// dialog to show when the object is locked and we DON'T have the ULockItem
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName LockedDlg = NAME_None;

	// dialog to show when the object is triggered. in case of a locked object this happens after the ULockDlg
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName TriggerDlg = NAME_None;

	// setting this will reward the item on trigger and self-destruct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", AssetRegistrySearchable)
	FName ItemReward = NAME_None;

	// setting this will reward a flag on trigger, adding 1 *each* time. (won't self-destruct due to this variable)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", AssetRegistrySearchable)
	FName FlagReward = NAME_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float TriggerFlashInc = 0;

	// whether or not to fade when rewarding an item.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool UseAnimFade = true;
	
	// whether or not to auto destroy on item reward
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool AutoDestroy = true;
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ItemRewarded();
	virtual void ItemRewarded_Implementation();

	UFUNCTION()
	void Faded(); // called when the item reward fade ends
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;
	virtual bool TryTrigger_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;

	// used for fading the object on item reward.
	UPROPERTY(BlueprintReadOnly, Category="SetUp")
	UCAnimatorFade* AnimFade = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UFlags* Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDiags* Dialogs = nullptr;
};

// TODO at some point *consider* moving the ItemReward functionality to its own child class
// e.g. animfade, itemreward, useanimfade(redundant), autodestroy, itemrewarded,  flagrewarded