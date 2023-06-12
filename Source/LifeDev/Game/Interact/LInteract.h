#pragma once

#include "CoreMinimal.h"
#include "Interact/InteractAnim.h"

#include "LInteract.generated.h"
// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteract: public AInteractAnim {
public:
	GENERATED_BODY()

	// name of the item that will unlock this. setting it will lock the actor on start.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName ULockItem = NAME_None;

	// dialog to display if this object is locked AND we have the ULockItem. Not setting it will result in using LockDlg 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName LockItemDlg = NAME_None;

	// dialog to show when the object is locked and we DON'T have the ULockItem
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName LockDlg = NAME_None;

	// Dialog to show when unlocking, or none to not say anything
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName ULockDlg = NAME_None;

	// played when triggering on locked
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	USoundBase* SFX_Locked = nullptr;

	// setting this will reward the item on trigger and self-destruct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName ItemReward = NAME_None;

protected:
	virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
};