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
	void LearnShow(const FName& Id, const FTeachRow& Row);
	UFUNCTION(BlueprintCallable)
	void LearnHide(const FName& Id);

	// sets the prompt but doesn't show if it's not currently shown.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void PromptSet(const FText& Text);
	// shows the prompt and sets it.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction, AutoCreateRefTerm=Text))
	void PromptShow(const FText& Text);
	// hides the prompt
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float TeachAnimSpeed = 2; // 500ms
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float PromptAnimSpeed = 4; // ms

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Prompt = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> Pointer = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidget> PromptBG = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TTime = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TState = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGridPanel> GStatus = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<URichTextBlock> TTeach = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidget> TeachBG = nullptr;
	// it HAS to be transient or the widget bp won't compile
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ATeachIn = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> APromptIn = nullptr;
};
