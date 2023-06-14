#pragma once

#include "CoreMinimal.h"
#include "Dialogs/Dialogs.h"
#include "Interact/InteractAnim.h"
#include "Inventory/Inventory.h"

// TODO extract part of this into a class parent of this and LInteract
// maybe the item reward part

#include "LInteractSpot.generated.h"
// A Place to drop other interacts
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALInteractSpot: public AInteract {
public:
	GENERATED_BODY()

	// ALInteractSpot();
	
	// When the reward is triggered
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName TriggerDlg = NAME_None;

	// When a drop is correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName CorrectDlg = NAME_None;

	// When a drop is correct
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName FullDlg = NAME_None;

	// the item to reward
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName ItemReward = NAME_None;

	// items to listen to
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FName> Items;

	// texts, for not full and full
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<FText> Texts = {
		FText::FromString(TEXT("Drop here")),
		FText::FromString(TEXT("Full"))
	};

protected:
	virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	virtual void SetText_Implementation() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
};
