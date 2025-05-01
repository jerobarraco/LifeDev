// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBaseUIDoneVal, const int32, RetVal);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UBaseUI: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Show();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Hide();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done(const int32 RetVal = 0);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool ShowCursor = false;

	// will auto-unbind from done on hide
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool AutoUnbind = true;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FBaseUIDone OnDone;
	UPROPERTY(BlueprintAssignable, EditAnywhere, Transient)
	FBaseUIDoneVal OnDoneVal;
};
