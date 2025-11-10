// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "DiagTypes.h"

#include "DiagMan.generated.h"

class UDiags;
class UDialogUI;
class UInputMappingContext;
class UInputAction;

// The in game manager for dialogs. can be subclassed/extended.
UCLASS(Blueprintable, BlueprintType, Config=Diag, DefaultConfig)
class DIAGS_API ADiagMan : public AInfo {
	GENERATED_BODY()

public:
	ADiagMan();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ADiagMan* Instance(const UObject* const O);

	// Called by the gamemode or god class
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Init();
	// Called by the gamemode or god class. (or endplay)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DeInit();

	// show a dialog. Called automatically from the Diag subsystem.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Show(const FDiag& Diag);
	// When a dialog is added (before it's shown). Called automatically from the Diag subsystem.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Add(const FName& Name, const FDiag& Diag);
	// Diags are done showing Diags (no more Diags). Called automatically from the Diag subsystem.// note that there's an animation after this before the ui is hidden. Override "Hidden" for that.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DiagDone();

	// attempt to skip the current dialog
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Skip();
	// Attempts to go back
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Back();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void AutoStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void AutoStop();

	// whether the ui is showing
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsShowing() const { return IsShowing; }

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	int32 InputPrio = 10;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp, Config)
	int32 ZOrder = 3;
	// when set, will force use of auto on all dialogs.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp", Config)
	bool UseAutoForce = false;
	static constexpr float DefAutoTime = 2.5;
	// how much to wait before trying to auto skip.
	// Requires feature flag D_AUTO
	// a very low value can break stuff.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SetUp", Config, meta=(ClampMin=.05))
	float AutoTime = DefAutoTime;

	// when unset (false) it will skip showing the Diags but still mark them as read
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Debug)
	bool UseShow = false;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UDialogUI> UIClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputMappingContext> Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionSkip = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionAuto = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionBack = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// The ui is done with the text. Has shown the last it knows.
	// The diag sub might still have some more diags.
	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void UIDone();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDialogUI> UI = nullptr;

	FTimerHandle AutoTimer;
private:
	bool IsShowing = false;
};
