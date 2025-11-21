// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Inventory/InventoryUI.h"

#include "LInventoryUI.generated.h"

class UScrollBox;
class ULInventoryItemUI;

UCLASS(Blueprintable)
class LIFEDEV_API ULInventoryUI : public UInventoryUI {
	GENERATED_BODY()
public:
	ULInventoryUI();

	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;
	virtual void SetItemMod_Implementation(const FName& Name, int32 Diff, const FItem& Item) override;
	virtual void SetSelected_Implementation(const FName& Name) override;
	virtual void SetItemUsed_Implementation(const FName& Name) override;
	virtual void SetItemCold_Implementation(const FName& Name) override;

	UFUNCTION(BlueprintCallable) // move to LInventory and use the new Items
	ULInventoryItemUI* GetItem(const FName& Name);
	// will update an item with the info from the inventory. item won't be created.
	UFUNCTION(BlueprintCallable)
	ULInventoryItemUI* ResetItem(const FName& Name);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Fade(const bool In = true);

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	
	void AddItem(UWidget* const It) const;
	UFUNCTION(BlueprintCallable, CallInEditor) // ufunction to be able to trigger from the console
	void ReorderItems();

	// default class to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ULInventoryItemUI> ItemClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Transient)
	TMap<FName, TObjectPtr<ULInventoryItemUI>> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> SItems = nullptr;
};
