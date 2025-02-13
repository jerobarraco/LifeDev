// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "JButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJButtonClick, const int32, Id);

// basic button with also an id. very helpful.
// create your own custom class in blueprint, and inherit from this, to give it some style.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UJButton: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=SetUp)
	void SetUp(const FText& NewText, const int32 NewId = -1);

	// will be called automatically. it's also exposed for testing.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DoClick();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 Id=-1;

	// bind to this, not the regular button click.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FJButtonClick OnClick;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> Text = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn = nullptr;
};
