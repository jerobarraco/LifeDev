// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "MsgBox.generated.h"

class UJButton;
class UDelegateWrapper;
class UTextBlock;
class UButton;

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UMsgBox: public UBaseUI {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(const FText& Message, const TArray<FText>& Texts);
	virtual void Show_Implementation() override;
	void HideAnimFinish();
	virtual void Hide_Implementation() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float AnimDuration = .5;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void BtnClick(int32 ID);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Msg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UJButton* Btn2 = nullptr;

	// has to be transient or it will not compile the bp
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	UWidgetAnimation* AnimShow = nullptr;

private:
	UPROPERTY(Transient)
	TArray<UDelegateWrapper*> Wrappers;
};
