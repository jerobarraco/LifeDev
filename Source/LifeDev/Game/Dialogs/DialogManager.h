// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "DiagTypes.h"
#include "LDialogs.h"

#include "DialogManager.generated.h"

class UDialogUI;

// base class for the character
UCLASS(Blueprintable, config=Game)
class LIFEDEV_API ADialogManager : public AActor {
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
	
	void HideUI() const;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UDialogUI> UIClass = nullptr;

private:
	// stub. the manager will handle input. will it?
	UFUNCTION()
	void UIFinished();
	
	UPROPERTY(Transient)
	ULDialogs* Dialogs = nullptr;

	UPROPERTY(Transient)
	UDialogUI* UI = nullptr;

	bool IsShowing = false;
};