// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "GameUI.generated.h"

class UTextBlock;

// The main ui for in-game
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UGameUI: public UUserWidget {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetPrompt(const FText& Text);

	UFUNCTION(BlueprintNativeEvent)
	void InteractShowPrompt(const FText& Text);

	UFUNCTION(BlueprintNativeEvent)
	void InteractHidePrompt();

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* T_Prompt = nullptr;
};
