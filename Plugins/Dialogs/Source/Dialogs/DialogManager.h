// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "DiagTypes.h"
#include "Dialogs.h"
#include "EnhancedInputComponent.h"

#include "DialogManager.generated.h"

class UDialogUI;
class UInputMappingContext;
class UInputAction;

// base class for the character
UCLASS(Blueprintable, config=Game)
class DIALOGS_API ADialogManager : public AActor {
	GENERATED_BODY()

public:
	ADialogManager();

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void DeInit();

	// show a dialog
	UFUNCTION(BlueprintCallable)
	void Show(const FDialog& Diag);

	// stop showing dialogs (no more dialogs)
	UFUNCTION(BlueprintCallable)
	void Hide();

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
	UInputMappingContext* Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionSkip = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionBack = nullptr;

	// when set it will skip showing the dialogs but still mark them as read
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category=Debug)
	bool DebugSkip = false;

private:
	// The ui is done with the current line
	UFUNCTION()
	void UIDiagDone();

	UPROPERTY(Transient)
	UDialogs* Dialogs = nullptr;

	UPROPERTY(Transient)
	UDialogUI* UI = nullptr;

	UPROPERTY(Transient)
	bool IsShowing = false;
};
