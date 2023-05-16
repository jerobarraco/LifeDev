// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogUIDone);

// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UDialogUI: public UUserWidget {
public:
	GENERATED_BODY()
	
	UFUNCTION(BlueprintNativeEvent)
	void Show(const FDialog& Diag);

	UFUNCTION(BlueprintNativeEvent)
	void Hide();

	// skips animation or the current dialog. will trigger an ondone if so.
	UFUNCTION(BlueprintNativeEvent)
	void Skip();

	// variable that indicates when the dialog is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsReady = false;
	
	// this is stub. the manager will handle this.
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FDialogUIDone OnDone;
};
