// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "DiagTypes.h"

#include "DiagMan.generated.h"

class UDiags;
class UDialogUI;
class UInputMappingContext;
class UInputAction;

// base class for the character
UCLASS(Blueprintable)
class DIAGS_API ADiagMan : public AInfo {
	GENERATED_BODY()

public:
	ADiagMan();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();

	// show a dialog
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Show(const FDialog& Diag);
	// stop showing Diags (no more Diags)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Hide();
	// whether the ui is showing
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsShowing() const { return IsShowing; }

	// attempt to skip the current dialog
	UFUNCTION(BlueprintCallable)
	void Skip();
	UFUNCTION(BlueprintCallable)
	void Back();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 3;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UDialogUI> UIClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputMappingContext> Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionSkip = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionBack = nullptr;

	// when set it will skip showing the Diags but still mark them as read
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Debug)
	bool DebugSkip = false;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDialogUI> UI = nullptr;

private:
	// The ui is done with the current line
	UFUNCTION()
	void UIDiagDone();

	bool IsShowing = false;
};
