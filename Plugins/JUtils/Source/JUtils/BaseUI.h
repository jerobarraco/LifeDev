// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseUIDone);

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
	void Done() {OnDone.Broadcast();}
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FBaseUIDone OnDone;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool ShowCursor = false;
};
