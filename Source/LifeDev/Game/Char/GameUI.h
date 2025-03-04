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
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void SetPrompt(const FText& Text);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void InteractShowPrompt(const FText& Text);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void InteractHidePrompt();

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Prompt = nullptr;
};
