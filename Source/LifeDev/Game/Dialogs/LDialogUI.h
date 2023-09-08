// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Dialogs/DialogUI.h"

#include "LDialogUI.generated.h"


// Helps define an interaction volume
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ULDialogUI: public UDialogUI {
	GENERATED_BODY()

public:
	ULDialogUI();

	virtual void NativeDestruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetTextMatProgress(float T);

protected:
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UMaterialParameterCollection* TextMPC = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UMaterialParameterCollectionInstance* TextMPCI = nullptr;
};
