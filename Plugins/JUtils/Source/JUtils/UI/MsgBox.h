// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "MsgBox.generated.h"

class UTextBlock;
class UButton;

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UMsgBox: public UBaseUI {
	GENERATED_BODY()

public:
	void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void ShowButtons(const TArray<FText> Texts);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float AnimDuration = .5;

protected:
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
	
};
