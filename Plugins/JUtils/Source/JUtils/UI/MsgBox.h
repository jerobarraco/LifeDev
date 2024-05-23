// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "MsgBox.generated.h"

class UJButton;
class UDelegateWrapper;
class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMsgBoxHidden);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UMsgBox: public UBaseUI {
	GENERATED_BODY()

public:
	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void Init(const FText& Message, const TArray<FText>& Texts);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float AnimDuration = .5;

	UPROPERTY(BlueprintAssignable, Transient)
	FMsgBoxHidden OnHidden;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void HideAnimFinish();
	
	UFUNCTION()
	void BtnClick(const int32 ID);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Msg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn2 = nullptr;

	// has to be transient, or it will not compile the bp
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	UWidgetAnimation* AnimShow = nullptr;

private:
	UPROPERTY(Transient)
	TArray<UDelegateWrapper*> Wrappers;

	UPROPERTY(Transient)
	FWidgetAnimationDynamicEvent OnHideFinished;
};
