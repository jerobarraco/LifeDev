// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"
#include "DiagTypes.h"

#include "DiagMan.generated.h"

class UDiags;
class UDialogUI;
class UInputMappingContext;
class UInputAction;

// The in game manager for dialogs. can be subclassed/extended.
UCLASS(Blueprintable)
class DIAGS_API ADiagMan : public AInfo {
	GENERATED_BODY()

public:
	ADiagMan();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ADiagMan* Instance(const UObject* const O);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Init();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DeInit();

	// show a dialog. Called automatically from the Diag subsystem.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Show(const FDiag& Diag);
	// When a dialog is added (before it's shown). Called automatically from the Diag subsystem.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Add(const FName Name, const FDiag& Diag);
	// stop showing Diags (no more Diags). Called automatically from the Diag subsystem.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Hide();

	// whether the ui is showing
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsShowing() const { return IsShowing; }

	// attempt to skip the current dialog
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Skip();
	// Attempts to go back
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
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
	// The ui is done with the current line
	UFUNCTION(BlueprintNativeEvent)
	void DiagDone();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDialogUI> UI = nullptr;

private:
	bool IsShowing = false;
};
