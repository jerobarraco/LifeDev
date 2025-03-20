// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "LInventoryItemUI.generated.h"

struct FItem;

UCLASS(Blueprintable)
class LIFEDEV_API UInventoryItemUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetItem(const FName& Name, const FItem& Item);

	// Called when the item is used
	UFUNCTION(BlueprintCallable)
	void Use();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FText GetItemCountText(const FItem& Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const FSlateColor& GetItemColor(const FItem& Item);

protected:
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AUse = nullptr; // has te be transient or it won't compile.
};
