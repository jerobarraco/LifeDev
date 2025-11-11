#pragma once

#include "CoreMinimal.h"
#include "Interact/InteractAnim.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LInteract.generated.h"

class UStory;
class UFlags;
class UCAnimatorFade;
class UDiags;
class UInventory;
class UFlashback;

// An interactive actor that can have an animation
// Dialogs/Flags added see ConstDiags.h
UCLASS(Blueprintable, BlueprintType, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALInteract: public AInteractAnim {
	GENERATED_BODY()

public:
#pragma region base
	ALInteract();

	// will fade in/out the object. also sets active by default.
	// optionally will un/set the hidden flag.
	// SetHidden is deprecated.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(UnsafeDuringActorConstruction, ForceAsFunction))
	void Fade(const bool FadeIn = false, const bool SetHidden=true);

	virtual void SetState_Implementation(const int32 NewState) override;
	virtual void SetActive_Implementation(const bool Active = true) override;
	virtual bool TryTrigger_Implementation() override;
	// will fade if it's not constructing.
	virtual void SetActorHiddenInGame(const bool NewHidden) override;
	// will tweak the anim fps based on the fb.
	// beware it can make a performance mess if there are too many interacts. 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool UseAnimFBFPS = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="SetUp|Base")
	float FBAnimMin = 1/5.0;
	// note, this will override the default fps for the Anim on BeginPlay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category="SetUp|Base")
	float FBAnimMax = 1/60.0;
#pragma endregion
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
		// rewardActor==nullptr works because both TSoftObjectPtr and FSoftObjectPtr overrides ==nullptr
		// i could have used IsNull, but this ties me a bit less to TSoftObjectPtr in particular.
		const bool Rewardless = (ZeroFlash & (RewardActor == nullptr) &
			RewardItem.IsNone() & (RewardIntersActive.Num() == 0) &
			(RewardIntersHint.Num() == 0) & (RewardIntersTrigger.Num() ==0));
			// note: use all rewards here. since it's confusing having to track which rewards destroys and which don't.
			// you might want to mix rewards that destroys and don't, that's fine. The result is to destroy.
			// you might want to not destroy, then use UseRewardDestroy=false.
		return Rewardless;
	}

	// setting this will reward the item on trigger.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward", AssetRegistrySearchable)
	FName RewardItem = NAME_None;

	// the mod value for the flash system when it's triggered.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	float RewardFlash = 0;

	// An actor to reward. Will be set hidden on begin play, and unhide on reward.
	// will fade if it's an LInteract.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Reward")
	TSoftObjectPtr<AActor> RewardActor = nullptr;
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
	// will deactivate if already triggered. by checking the flags, on begin play.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Base")
	bool UseActiveOnce = false;
#pragma endregion
#pragma region lock
	virtual bool ShouldUnlock_Implementation() override;
	virtual void Unlock_Implementation() override;

	// items to receive/consume in order to unlock.
	// items will be removed from this list on runtime whenever they are consumed.
	// if the item is consumable, it will be consumed. otherwise it will only check for presence.
	// Not using UnlockCondition here since i want to consume these items.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Lock", AssetRegistrySearchable)
	TArray<FName> UnlockItems;
#pragma endregion

	// used for fading this object on rewards or whenever you want.
	// remember to call SetNewMat on the constructor if you use the new material.
	// remember to call CreateMaterial on BeginPlay if there are no rewards.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorFade> AnimFade = nullptr; // cdo

protected:
#pragma region base
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void DoTrigger_Implementation() override;
	virtual void DoTriggerLocked_Implementation() override;
#pragma endregion
#pragma region Reward
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
#pragma endregion

	// called when a fade ends that wants to set the object as hidden.
	UFUNCTION() // bound
	void HideAfterFade();

#pragma region fbanim
	UFUNCTION()
	void FeatUpdG(const EFeat Feat, const bool Enabled);
	UFUNCTION()
	void FBUpd(const float Value);
#pragma endregion

#pragma region cache
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
#pragma endregion
};

// maybe at some point and these. only if i need them

// when true, the items can only be dropped in order.
// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
// bool UseOrder = false;

// when true it will increase the state on each usage
// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
// bool UseStateInc = false;


// not moving the reward stuff to another component.
// No:  is not that much code. is almost always used. it will have overhead. it couples tightly with timings and such.
// e.g. animfade, RewardItem, useanimfade(redundant), autodestroy, RewardItemed,  flagrewarded
