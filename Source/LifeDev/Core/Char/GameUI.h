// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "GameUI.generated.h"

// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UGameUI: public UUserWidget {
public:
	GENERATED_BODY()
	
	UFUNCTION(BlueprintNativeEvent)
	void SetPrompt(const FText& text);

	UFUNCTION(BlueprintNativeEvent)
	void InteractShowPrompt(const FText& Text);
	virtual void InteractShowPrompt_Implementation(const FText& Text) {};

	UFUNCTION(BlueprintNativeEvent)
	void InteractHidePrompt();
	virtual void InteractHidePrompt_Implementation() {};
};
