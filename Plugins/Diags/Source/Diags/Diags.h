// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DiagTypes.h"

#include "Diags.generated.h"

class UEval;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiagOnShow, const FDiag&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDiagOnAdd, const FName&, Name, const FDiag&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiagOnDone);

// World subsystem to deal with dialogs
UCLASS(Blueprintable, Category="Diags", Config="Diags", DefaultConfig)
class DIAGS_API UDiags : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static UDiags* Instance(const UObject* const O);

#pragma region Add
	// Main function. Use this.
	// Attempts to add a group id. Otherwise, it will attempt to add a dialog id.
	// Groups can contain other groups, so this could be recursive or cyclic (and might crash). Beware!
	// If, and only if, a *Sequence* id ends with '*' it will add a random one instead of a regular sequence (all the dialogs in it).
	// If a dialog ends with "*" it will simply add it.
	// This is on purpose to support certain code that can either trigger a random dialog in a sequence, or a specific dialog.
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool AddId(const FName& Row);
	// add many sequences or dialogs. can be recursive
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool AddIdMany(const TArray<FName>& Rows);

	// add a dialog by its id
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool AddDiagId(const FName& Row);
	// add a dialog
	UFUNCTION(BlueprintCallable, Category="Diags")
	void AddDiag(const FDiag& Diag);
	
	// add a sequence by id.
	// Sequence ids can contain other sequences, so this could be recursive or cyclic. Beware!
	// Sequences ending with "*" will only add one *random* dialog from that sequence.
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool AddGroupId(const FName& RowName);
	// Adds a sequence. Use this to AddManyById.
	// Beware this doesn't protect you from recursive sequences.
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool AddGroup(const FDiagGroup& Seq);
#pragma endregion
#pragma region Get
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool GetDiag(const FName& RowName, FDiag& OutRow) const;
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool GetChar(const FName& RowName, FDiagChar& OutChar) const;
	UFUNCTION(BlueprintCallable, Category="Diags")
	bool GetGroup(const FName& RowName, FDiagGroup& OutGroup) const;

	// true when there's a dialog showing
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsShowing() const { return IsShowing; }
#pragma endregion

	// called by the dialog manager when a dialogue is done showing
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DiagDone();

	// EXPERIMENTAL
	// Tests a Group's condition. Returns true if >0.
	// See the Eval subsystem for docs.
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="Diags")
	bool CheckCondition(const FString& Expression, double& Res) const;
	// left here in case someone needs to check the condition of a group for some weird reason.

#pragma region Init

	// todo find better names
	
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DTCharSet(UDataTable* const DT);
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DTDiagAdd(UDataTable* const DT);
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DTDiagRem(UDataTable* const DT){ Diags.Remove(DT); }
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DTGroupAdd(UDataTable* const DT);
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DTGroupRem(UDataTable* const DT){ Groups.Remove(DT); }

	// initialize. called by the gamemode
	UFUNCTION(BlueprintCallable, Category="Diags")
	void Init();

	// de-initialize. called by the gamemode
	UFUNCTION(BlueprintCallable, Category="Diags")
	void DeInit();

	UPROPERTY(BlueprintReadWrite, Category="Diags", Config)
	bool UseWarning = false;
#pragma endregion

#pragma region Delegates
	// When a dialog is added. Beware, this is called before OnShow.
	// Please don't call Add directly from this, wait a frame.
	// Otherwise, you might break OnShow or cause stack overflows.
	UPROPERTY(BlueprintAssignable, Category="Diags")
	FDiagOnAdd OnAdd;

	// when a dialog needs to show. Avoid calling Add directly from this, wait a frame.
	UPROPERTY(BlueprintAssignable, Category="Diags")
	FDiagOnShow OnShow;

	// when all dialogs have finished showing.
	UPROPERTY(BlueprintAssignable, Category="Diags")
	FDiagOnDone OnDone;
#pragma endregion

protected:
	void ShowNext();
	void Stop();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> Chars = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UDataTable>> Diags;
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UDataTable>> Groups;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UEval> Eval = nullptr;
	
	TArray<FDiag> Pending;
	bool IsShowing = false;
};
