// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DiagTypes.h"
#include "Dialogs.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDiagOnShow, const FDialog&, Diag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiagOnStop);

// World subsystem to deal with dialogs
UCLASS(Blueprintable)
class DIALOGS_API UDialogs : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Add(const FDialog& Diag);

	UFUNCTION(BlueprintCallable)
	void AddSeq(const TArray<FDialog>& Seq);

	UPROPERTY(BlueprintAssignable)
	FDiagOnShow OnShow;
	
	UPROPERTY(BlueprintAssignable)
	FDiagOnStop OnStop;

	// called by the dialog manager when a dialogue is done showing
	UFUNCTION(BlueprintCallable)
	void DiagDone();

protected:

	void ShowNext();
	void Stop();
	
	FDialogSeq Pending;

	bool IsShowing = false;
};
