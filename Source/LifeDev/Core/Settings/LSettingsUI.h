// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "JUtils/UI/BaseUI.h"

#include "LSettingsUI.generated.h"

class ULSetDbgUI;
class UWidgetSwitcher;
class UButton;
class UComboBoxString;
class UGroupBox;

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
	virtual void Apply_Implementation(){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction, ForceAsFunction))
	void Load();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType);
	UFUNCTION()
	void ShowDbg();
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> CBScale;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UButton> BtnOptDbg;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> SWOptions;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULSetDbgUI> Settings_Dbg;
};
