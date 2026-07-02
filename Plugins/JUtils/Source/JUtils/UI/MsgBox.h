// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "BaseUI.h"
#include "JButton.h"

#include "MsgBox.generated.h"

class UHorizontalBox;
class UJButton;
class UDelegateWrapper;
class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMsgBoxHidden);
// Used as parameter for ShowNow
// unfortunately FBaseUIOnDoneVal::FDelegate doesn't work with UFUNCTION.
DECLARE_DYNAMIC_DELEGATE_OneParam(FMsgBoxOnDone, const int32, RetVal);

// Base message box.
// Will trigger OnDoneVal(ButtonId) on button clicked.
// will auto unbind from onDone and onDoneVal on hidden.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UMsgBox: public UBaseUI {
	GENERATED_BODY()

public:
	UMsgBox();
	
	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;

	// shows with the message, btntexts, and callback.
	// it will add the newOnDoneVal to the OnDoneVal
	UFUNCTION(BlueprintCallable, Category=SetUp, meta=(AutoCreateRefTerm=Message))
	void ShowNow(const FText& Message, const TArray<FText>& BtnTexts,
		const FMsgBoxOnDone& NewOnDoneVal);
	UFUNCTION(BlueprintCallable, Category=SetUp, meta=(AutoCreateRefTerm=Message))
	void SetUp(const FText& Message, const TArray<FText>& BtnTexts);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float AnimDuration = .5;

	// triggered once it has finished hiding.
	UPROPERTY(BlueprintAssignable, Transient)
	FMsgBoxHidden OnHidden;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	void Bind();
	void Unbind();
	void BtnsClear(const uint32 Reserve = 0);

	UFUNCTION()
	void HideAnimFinish();
	
	UFUNCTION()
	void BtnClick(const int32 ID);

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UJButton> BtnClass = UJButton::StaticClass();

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category=SetUp)
	int32 Pad = 10;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> Msg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> BtnBox = nullptr;

	// has to be transient, or it will not compile the bp
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> AnimShow = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UJButton>> Btns;

private:
	UPROPERTY(Transient) // internal use only.
	FWidgetAnimationDynamicEvent OnHideFinished;
};
