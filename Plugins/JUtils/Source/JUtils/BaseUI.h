// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

// TODO move to UI

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBaseUIDoneVal, int32, RetVal);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UBaseUI : public UUserWidget {
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Show();
	void Show_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hide();
	void Hide_Implementation();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done(int32 RetVal = 0);
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FBaseUIDone OnDone;
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FBaseUIDoneVal OnDoneVal;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool ShowCursor = false;
};
