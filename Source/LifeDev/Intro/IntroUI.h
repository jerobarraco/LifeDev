// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "IntroUI.generated.h"

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
	void ShowSettings(const int32 Id);

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FIntroUIDone OnDone;
	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FIntroUISlotsDone OnSlotsDone;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SlotsLoadDone(const bool HasDoneSave);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UMsgBox> MsgBox = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> Switcher = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnNext = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnNext2 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnSettings = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnDone = nullptr;
	// UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	// TObjectPtr<UJButton> BtnNext = nullptr;
	// UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	// TObjectPtr<UJButton> BtnNext = nullptr;
};
