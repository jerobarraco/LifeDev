// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "BaseUI.h"

#include "OutroUI.generated.h"

class UJButton;
class UWidgetSwitcher;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIQuit);

UCLASS(Blueprintable)
class LIFEDEV_API UOutroUI : public UBaseUI {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetFlags(const TMap<FName, float>& Flags);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetPage(const int32 Page=0);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> Switcher = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnRetry = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnCredits = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnStats = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnQuit = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnBack = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnBack2 = nullptr;
};
