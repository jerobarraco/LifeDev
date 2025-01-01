#pragma once

#include "CoreMinimal.h"
#include "Interact/InteractAnim.h"

#include "LInteract.generated.h"

class UStory;
class UFlags;
class UCAnimatorFade;
class UDiags;
class UInventory;
class UFlashback;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteract: public AInteractAnim {
	GENERATED_BODY()

public:
	ALInteract();

	// will fade in/out the object. also sets active by default.
	// optionally will un/set the hidden flag.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(UnsafeDuringActorConstruction))
	void Fade(const bool FadeIn = false, const bool SetHidden=false);

#pragma region rewards
	// returns true if this object is set to perform a reward and destroy.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool WillRewardDestroy() const {
		return UseRewardDestroy && !IsRewardless();
	}

	// returns true if it does not have a reward (yes, i know about negations)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsRewardless() const {
		const bool ZeroFlash = FMath::IsNearlyZero(RewardFlash);
		const bool Rewardless = (!RewardStep && ZeroFlash &&
			RewardActor == nullptr &&
			RewardItem.IsNone() && RewardFlag.IsNone() &&
			// also taking this into account for animation and logic purposes
			RewardIntersActive.Num() == 0);
		return Rewardless;
	}

	// setting this will reward the item on trigger.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward", AssetRegistrySearchable)
	FName RewardItem = NAME_None;
	// setting this will reward a flag on trigger, adding 1 *each* time.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward", AssetRegistrySearchable)
	FName RewardFlag = NAME_None;
	// the mod value for the flash system when it's triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	float RewardFlash = 0;

	// An actor to reward. Will be set hidden on begin play, and unhide on reward.
	// will fade if it's an LInteract.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TObjectPtr<AActor> RewardActor = nullptr;

	// Will start the next story step (finishing the current one).
	// called reward so that the UseRewardDestroy affects it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	bool RewardStep = false;
	// whether to self-destroy when *rewarding* (only if rewards are set).
	// if UseFade is true AND has something to reward, it will also fade.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	bool UseRewardDestroy = true;
	// Whether to use fade at all. if set it will *create* a dynamic material instance.
	// uses the AnimFade object and what's set there.
	// Remember to call AnimFade->SetNewMat on the _constructor_ if you use the new material.
	// By default, it's false. Other-wise it will cost performance.
	// (will replace your material and incur in extra draw calls).
	// Important to set it when using the Reward stuff and UseRewardDestroy.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	bool UseFade = false; 
#pragma endregion

	// name of the item that is needed to "have" to unlock this. (just having it will unlock it, unless we also set ULockItem)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock", AssetRegistrySearchable)
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName ULockDlg = NAME_None;
	// dialog to trigger when tried to use the wrong item to unlock this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName ULockBadDlg = NAME_None;
	// dialog to display if this object is locked AND we have the ULockItem. Not setting it will result in using LockDlg 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName LockedItemDlg = NAME_None;
	// dialog to show when the object is locked and we DON'T have the ULockItem
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName LockedDlg = NAME_None;
	// dialog to show when the object is triggered. in case of a locked object this happens after the ULockDlg
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg")
	FName TriggerDlg = NAME_None;
	// dialog to show when trying to use an item (just before actually triggering or trigger locked)
	// the key is the item name, the value is the dialog id.
	// this happens before trying to unlock.
	// do not specify the same item in UseItemsDlgs and ULockItem. use ULockDlg and ULockBadDlg for that.
	// This is a whitelist by design, since it will override trigger, trigger locked, and unlock.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Dlg", AssetRegistrySearchable)
	TMap<FName, FName> UseItemDlgs;

	// used for fading this object on rewards or whenever you want.
	// remember to call SetNewMat on the constructor if you use the new material.
	// remember to call CreateMaterial on BeginPlay if there are no rewards.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorFade> AnimFade = nullptr;

protected:
	// triggered when something is rewarded. override to be notified.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Rewarded();
	virtual void Rewarded_Implementation() {}

	// mostly internal. gives the rewards. if UseRewardDestroy it WILL self-destroy.
	// it won't fade, you'll have to do it manually.
	// exposed in case you want to do multiple rewards, in which case don't set UseRewardDestroy
	UFUNCTION(BlueprintCallable)
	void DoRewards();

	// called when the item reward fade ends. it WILL destroy the object.
	UFUNCTION() // bound
	void DestroyAfterReward();

	// called when a fade ends that wants to set the object as hidden.
	UFUNCTION() // bound
	void HideAfterFade();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void DoTrigger_Implementation() override;
	virtual void DoTriggerLocked_Implementation() override;
	virtual bool TryTrigger_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Item) override;

	/// cache
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlashback> Flashback = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStory> Story = nullptr;
};

// TODO at some point *consider* moving the Reward functionality to its own child class
// e.g. animfade, RewardItem, useanimfade(redundant), autodestroy, RewardItemed,  flagrewarded
//No:  is not that much code. is almost always used. it will have overhead