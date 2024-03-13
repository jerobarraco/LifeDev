// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBaseUIDoneVal, int32, RetVal);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UBaseUI: public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Show();
	virtual void Show_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hide();
	virtual void Hide_Implementation();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done(int32 RetVal = 0);
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FBaseUIDone OnDone;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FBaseUIDoneVal OnDoneVal;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	bool ShowCursor = false;
};
