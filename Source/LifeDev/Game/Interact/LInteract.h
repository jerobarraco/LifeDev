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

	UFUNCTION(BlueprintCallable)
	void Fade(bool FadeIn = false);
	
	// name of the item that is needed to "have" to unlock this. (just having it will unlock it, unless we also set ULockItem)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockItemReq = NAME_None;
		
	// name of the flag that is needed to "have" to unlock this.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock")
	FName ULockFlagReq = NAME_None;
	
	// *Using* this item with this instance will unlock it. setting it will lock the actor on start.
	// it will also decide whether to show LockedDlg or LockedItemDlg on trigger(locked)
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

	// setting this will reward the item on trigger. will self-destroy if DestroyOnReward is set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", AssetRegistrySearchable)
	FName RewardItem = NAME_None;

	// setting this will reward a flag on trigger, adding 1 *each* time. will self-destroy if DestroyOnReward is set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", AssetRegistrySearchable)
	FName RewardFlag = NAME_None;

	// the mod value for the flash system when it's triggered. will self-destroy if DestroyOnReward is set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float RewardFlash = 0;

	// An actor to reward. will self-destroy if DestroyOnReward is set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	AActor* RewardActor = nullptr;

	// whether or not to fade when rewarding an item.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool UseRewardFade = true;
	
	// whether or not to auto destroy on rewarded
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool UseRewardDestroy = true;
	
protected:
	// triggered when item is rewarded
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Rewarded();
	virtual void Rewarded_Implementation();

	// called when the item reward fade ends
	// not using rewardfaded since the latter will destroy the object and will confuse clients.
	UFUNCTION() // bound
	void RewardFaded();
	
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

// TODO at some point *consider* moving the RewardItem functionality to its own child class
// e.g. animfade, RewardItem, useanimfade(redundant), autodestroy, RewardItemed,  flagrewarded
//No:  is not that much code. is almost always used. it will have overhead