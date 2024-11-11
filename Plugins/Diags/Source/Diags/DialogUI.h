// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "BaseUI.h"

#include "DiagTypes.h"

#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogUIDone);

// note, the porting of this from bp to cpp is still WIP

// Base UI class for dialogs
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAGS_API UDialogUI: public UBaseUI {
	GENERATED_BODY()
	
public:
	UDialogUI();

	// shows or queues a new dialog
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowDlg(const FDialog& Diag);
	
	// skips animation or the current dialog. will trigger an onDone if already ready.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Skip();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Back();
	// Skips back and forth.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SkipBy(const int32 Diff = 1);

protected:
	virtual void Hide_Implementation() override;
	virtual void Show_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowCurDlg();

	UFUNCTION(BlueprintCallable)
	void PostHide();

	// could be ==Dlgs.Num() when expecting a new dialog.
	UPROPERTY(BlueprintReadWrite, Transient)
	int32 CurDlgI = 0;

	UPROPERTY(BlueprintReadWrite, Transient)
	TArray<FDialog> Dlgs;
};
