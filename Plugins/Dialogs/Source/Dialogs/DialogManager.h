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
	
	UFUNCTION(BlueprintCallable)
	void Show(const FDialog& Diag);

	UFUNCTION(BlueprintCallable)
	void Stop();
	
	UFUNCTION(BlueprintCallable)
	void Skip();
	
	void HideUI() const;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UDialogUI> UIClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputMappingContext* Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionSkip = nullptr;

private:
	// stub. the manager will handle input. will it?
	UFUNCTION()
	void DiagDone();
	
	UPROPERTY(Transient)
	UDialogs* Dialogs = nullptr;

	UPROPERTY(Transient)
	UDialogUI* UI = nullptr;

	UPROPERTY(Transient)
	bool IsShowing = false;
};