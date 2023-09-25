#pragma once

#include "CoreMinimal.h"
#include "LInteract.h"

#include "LInteractReward.generated.h"

class UCAnimatorFade;
// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractReward: public ALInteract {
	GENERATED_BODY()

public:
	ALInteractReward();
	// setting this will reward the item on trigger and self-destruct
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp", AssetRegistrySearchable)
	// FName ItemReward = NAME_None;

	// whether or not to fade when rewarding an item.
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	// bool UseAnimFade = true;
	
	// whether or not to auto destroy on item reward
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	// bool AutoDestroy = true;
	
protected:
	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// void ItemRewarded();
	// virtual void ItemRewarded_Implementation();

	// UFUNCTION()
	// void Faded(); // called when the item reward fade ends
	
	// virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;
	// virtual void TriggerLocked_Implementation() override;
	// virtual bool TryTrigger_Implementation() override;
	// virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;

	// used for fading the object on item reward.
	// UPROPERTY(BlueprintReadOnly, Category="SetUp")
	// UCAnimatorFade* AnimFade = nullptr;
};

// TODO at some point *consider* moving the ItemReward functionality to its own child class
// e.g. animfade, itemreward, useanimfade(redundant), autodestroy, itemrewarded