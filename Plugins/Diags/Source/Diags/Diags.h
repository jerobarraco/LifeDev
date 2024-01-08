// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DiagTypes.h"

#include "Diags.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiagOnShow, const FDialog&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiagOnDone);

// World subsystem to deal with dialogs
UCLASS(Blueprintable, Category="Dialogs")
class DIAGS_API UDiags : public UWorldSubsystem {
	GENERATED_BODY()

public:
	static UDiags* Get(UWorld* World);
	
	// Attempts to add a sequence id. otherwise it will attempt to add a dialog id.
	// sequence ids can contain other sequences, so this could be recursive.
	// if and only if a name ends with '*' it will add a random one instead of a regular sequence.
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddId(const FName& Row);
	// add many sequences or dialogs. can be recursive
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddManyIds(const TArray<FName>& Rows);

	// add a dialog by its id
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddDiagId(const FName& Row);
	// add a dialog
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void AddDiag(const FDialog& Diag);

	// add a sequence by id. a sequence can contain other sequences ids, so this could be recursive.
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddSeqId(const FName& RowName);
	// Adds a sequence. Use this to AddManyById
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddSeq(const FDialogSequence& Seq);
	// From a sequence adds one random. This will call AddID so the sequence can contain other sequences. and other random ones.
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool AddRnd(const FDialogSequence& Seq);

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
	void SetData(UDataTable* Diags, UDataTable* Chars, UDataTable* Seqs);

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void Init();

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void DeInit();

	// true when there's a dialog showing
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsShowing() { return IsShowing; }

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
