// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "DiagTypes.h"

#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogUIDone);

// Base UI class for dialogs
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAGS_API UDialogUI: public UUserWidget {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Show(const FDialog& Diag);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hide();

	// skips animation or the current dialog. will trigger an onDone if already ready.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Skip();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Back();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowDlg(int Diff = 1);

	// triggers when the ui has done showing the current dialog
	UPROPERTY(BlueprintAssignable, EditAnywhere)
	FOnDialogUIDone OnDone;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();

	// Call when the ui finished showing the current dialog sequence
	UFUNCTION(BlueprintCallable)
	void Done() { OnDone.Broadcast(); }

	// TODO port
	UPROPERTY(BlueprintReadWrite, Transient)
	int32 CurDlgI = -1;
};
