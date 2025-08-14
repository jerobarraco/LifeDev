// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Inventory/InventoryUI.h"

#include "LInventoryUI.generated.h"

UCLASS(Blueprintable)
class LIFEDEV_API ULInventoryUI : public UInventoryUI {
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void FadeUsed(const bool Fwd=true);

	virtual void Show_Implementation() override;
	
	virtual void SetItemMod_Implementation(const FName& Name, int32 Diff, const FItem& Item) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetAnimOptional), Transient) // it HAS to be transient, or it will fail compilation.
	TObjectPtr<UWidgetAnimation> AUsed = nullptr;
};
