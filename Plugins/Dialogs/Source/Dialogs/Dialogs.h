// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DiagTypes.h"
#include "Dialogs.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiagOnShow, const FDialog&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiagOnDone);

// World subsystem to deal with dialogs
UCLASS(Blueprintable, Category="Dialogs")
class DIALOGS_API UDialogs : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void Add(const FDialog& Diag);
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddId(const FName& Row, FDialog& OutDialog, FDialogChar& OutChar);
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void AddMany(const TArray<FDialog>& Seq);

	// Use this to AddManyById
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddSeq(const FDialogSequence& Seq, TArray<FDialog>& OutDialogs, TArray<FDialogChar>& OutChars);
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddSeqId(const FName& RowName, FDialogSequence& OutSeq, TArray<FDialog>& OutDiags, TArray<FDialogChar>& OutChars);

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool GetDiag(const FName& RowName, FDialog& OutRow, FDialogChar& OutChar) const;
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool GetChar(const FName& RowName, FDialogChar& OutChar) const;
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool GetSeq(const FName& RowName, FDialogSequence& OutSeq) const;

	// called by the dialog manager when a dialogue is done showing
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void DiagDone();

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void Init(UDataTable* Diags, UDataTable* Chars, UDataTable* Seqs);

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void DeInit();

	// when a dialog needs to show
	UPROPERTY(BlueprintAssignable, Category="Dialogs")
	FDiagOnShow OnShow;

	// when there are no more dialogs to show now
	UPROPERTY(BlueprintAssignable, Category="Dialogs")
	FDiagOnDone OnDone;

protected:

	void ShowNext();
	void Stop();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UDataTable* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UDataTable* Chars = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UDataTable* Seqs = nullptr;
	
	TArray<FDialog> Pending;
	bool IsShowing = false;
};
