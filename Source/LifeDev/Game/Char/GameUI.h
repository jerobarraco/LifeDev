// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "GameUI.generated.h"

class UImage;
class UTextBlock;

// The main ui for in-game
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UGameUI: public UUserWidget {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void SetPrompt(const FText& Text);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void PromptShow(const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void PromptHide();

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetPointerShow(const bool Vis);

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Prompt = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Pointer = nullptr;
};
