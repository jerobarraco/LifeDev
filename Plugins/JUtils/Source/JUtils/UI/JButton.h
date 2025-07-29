// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "JButton.generated.h"

class USlateBrushAsset;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJButtonClick, const int32, Id);

// done this way (with a composed button and not inheriting it, since inheriting from UserWidget is the only way to be able to access the designer and style it)

// basic button with also an id. very helpful.
// create your own custom class in blueprint, and inherit from this, to give it some style.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UJButton: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=SetUp, meta=(AutoCreateRefTerm=NewText))
	void SetUp(const FText& NewText, const int32 NewId = -1);
	UFUNCTION(BlueprintCallable, Category=SetUp)
	void SetStyle(USlateWidgetStyleAsset* const BtnStyle, USlateWidgetStyleAsset* const TextStyle);

	// will be called automatically. it's also exposed for testing.
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintNativeEvent)
	void DoClick();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UButton* GetBtn() const { return Btn;}

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 Id = -1;

	// bind to this, not the regular button click.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FJButtonClick OnClick;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION() // in order to bind a virtual method
	void DoClickInt() {DoClick();}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> Text = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Btn = nullptr;
};
