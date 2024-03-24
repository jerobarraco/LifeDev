// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "MsgBox.generated.h"

class UDelegateWrapper;
class UTextBlock;
class UButton;

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UMsgBox: public UBaseUI {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(const FText& Message, const TArray<FText>& Texts);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float AnimDuration = .5;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void BtnClick(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Msg = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Btn0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Btn1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* Btn2 = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UTextBlock* Text0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UTextBlock* Text1 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UTextBlock* Text2 = nullptr;

private:
	UPROPERTY(Transient)
	TArray<UDelegateWrapper*> Wrappers;
};
