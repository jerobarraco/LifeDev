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
	
	UFUNCTION(BlueprintCallable)
	void FadeUsed(const bool Fwd=true);

	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;
	
	virtual void SetItemMod_Implementation(const FName& Name, int32 Diff, const FItem& Item) override;

protected:
	// default class to use
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ULInventoryItemUI> ItemClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Transient)
	TMap<FName, TObjectPtr<ULInventoryItemUI>> Items;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> SItems = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetAnimOptional), Transient) // it HAS to be transient, or it will fail compilation.
	TObjectPtr<UWidgetAnimation> AUsed = nullptr;
};
