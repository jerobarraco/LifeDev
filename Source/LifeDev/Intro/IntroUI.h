// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "IntroUI.generated.h"

class ULSaveGroup;
class UWidgetSwitcher;
class UJButton;
class UMsgBox;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIntroUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIntroUISlotsDone, const bool, HasDoneSave);

UCLASS(Blueprintable)
class LIFEDEV_API UIntroUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done(const int32 Id=0) {OnDone.Broadcast();}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void ShowMsg(const FText& Msg);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void ShowSettings();

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FIntroUIDone OnDone;
	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FIntroUISlotsDone OnSlotsDone;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	UFUNCTION()
	virtual void DoSettings(int32 I) {ShowSettings();}
	UFUNCTION(BlueprintCallable)
	void SlotsLoadDone(const bool HasDoneSave);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UMsgBox> MsgBox = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> Switcher = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnNext = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnBack = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnQuit = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnSettings = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnDone = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULSaveGroup> SaveGroup = nullptr;
};
