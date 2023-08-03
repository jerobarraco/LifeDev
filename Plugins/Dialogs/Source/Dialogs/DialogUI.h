// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "DiagTypes.h"

#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogUIDone);

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
	UPROPERTY(BlueprintReadWrite, meta=(DeprecatedProperty))
	bool IsReady = false;

	// triggers when the ui has done showing the current dialog
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FOnDialogUIDone OnDone;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();

	// Call when the ui finished showing the current dialog
	UFUNCTION(BlueprintCallable)
	void Done() { OnDone.Broadcast();}
};
