// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "BaseUI.h"

#include "DiagTypes.h"

#include "DialogUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogUIDone);

// Base UI class for dialogs
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIAGS_API UDialogUI: public UBaseUI {
	GENERATED_BODY()
	
public:
	UDialogUI();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowDlg(const FDialog& Diag);
	
	// skips animation or the current dialog. will trigger an onDone if already ready.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Skip();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Back();
	// Skips back and forth.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SkipBy(int Diff = 1);

protected:
	virtual void Hide_Implementation() override;
	virtual void Show_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void PostHide();

	// TODO port
	UPROPERTY(BlueprintReadWrite, Transient)
	int32 CurDlgI = -1;
};
