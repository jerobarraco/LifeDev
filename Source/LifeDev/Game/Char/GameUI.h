// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"
#include "Inventory/Teach/TeachTypes.h"

#include "GameUI.generated.h"

class URichTextBlock;
class AStep;
class UGridPanel;
class UImage;
class UTextBlock;

// The main ui for in-game
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UGameUI: public UUserWidget {
	GENERATED_BODY()
	
public:
	void Init();
	void DeInit();
	
	UFUNCTION(BlueprintCallable)
	void LearnShow(const FName& Id, const FLearnRow& Row);
	UFUNCTION(BlueprintCallable)
	void LearnHide();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void SetPrompt(const FText& Text);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void PromptShow(const FText& Text);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void PromptHide();

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetPointerShow(const bool Vis);

	void ShowStatus(const bool Enabled) const;

protected:
	UFUNCTION()
	void FlagMod(const FName& Name, const float Diff, const float Total);
	UFUNCTION()
	void StepStart(AStep* const Step);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Prompt = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> Pointer = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UWidget> PromptBG = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TTime = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TState = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGridPanel> GStatus = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<URichTextBlock> TLearn = nullptr;
};
