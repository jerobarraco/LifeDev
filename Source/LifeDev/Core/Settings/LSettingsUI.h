// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "JUtils/UI/BaseUI.h"

#include "LSettingsUI.generated.h"

class ULMsgBox;
class UEditableTextBox;
class UJButton;
class UTextBlock;
class ULSetDbgUI;
class UWidgetSwitcher;
class UButton;
class UComboBoxString;
class UGroupBox;

// the ui for the ingame settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettingsUI : public UBaseUI {
	GENERATED_BODY()

public:
	ULSettingsUI();

	// will reload the settings
	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction, ForceAsFunction))
	void Apply();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction, ForceAsFunction))
	void Load();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	UFUNCTION()
	void ShowDbg();
	UFUNCTION()
	void SetPause();
	UFUNCTION()
	void SendComment(const int32 Id);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> CBScale;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnOptDbg;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> SWOptions;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULSetDbgUI> Settings_Dbg;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TFoxy;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UEditableTextBox> TComment;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnComment;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULMsgBox> MsgBox;

	FTimerHandle PauseTimer;
	float PauseTime = .5;
};
