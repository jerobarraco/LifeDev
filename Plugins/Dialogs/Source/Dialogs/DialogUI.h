// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "DiagTypes.h"

#include "DialogUI.generated.h"

// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGS_API UDialogUI: public UUserWidget {
public:
	GENERATED_BODY()
	
	UFUNCTION(BlueprintNativeEvent)
	void Show(const FDialog& Diag);

	UFUNCTION(BlueprintNativeEvent)
	void Hide();

	// skips animation or the current dialog. will trigger an onDone if already ready.
	UFUNCTION(BlueprintNativeEvent)
	void Skip();

	// variable that indicates when the dialog is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsReady = false;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();
};
