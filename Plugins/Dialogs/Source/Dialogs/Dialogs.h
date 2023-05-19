// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DiagTypes.h"
#include "Dialogs.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiagOnShow, const FDialog&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiagOnStop);

// World subsystem to deal with dialogs
UCLASS(Blueprintable, Category="Dialogs")
class DIALOGS_API UDialogs : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void Add(const FDialog& Diag);

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void AddSeq(const TArray<FDialog>& Seq);

	// called by the dialog manager when a dialogue is done showing
	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void DiagDone();

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	void Load(UDataTable* AllDialogs);

	UFUNCTION(BlueprintCallable, Category="Dialogs")
	bool GetDiag(const FName& RowName, FDialog& OutRow);
	
	UPROPERTY(BlueprintAssignable, Category="Dialogs")
	FDiagOnShow OnShow;
	
	UPROPERTY(BlueprintAssignable, Category="Dialogs")
	FDiagOnStop OnHide;

protected:

	void ShowNext();
	void Stop();
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UDataTable* AllDiags = nullptr;
	FDialogSeq Pending;
	bool IsShowing = false;
};
