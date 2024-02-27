// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "OutroUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIDone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutroUIQuit);

// TODO reparent to BaseUI

UCLASS(Blueprintable)
class LIFEDEV_API UOutroUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Done() {OnDone.Broadcast();}
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Quit() {OnQuit.Broadcast();}
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FOutroUIDone OnDone;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FOutroUIQuit OnQuit;	
};
