// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "BaseUI.h"

#include "LOverlayUI.generated.h"


class UGroupBox;
class UTextBlock;
class UImage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ULOverlayUI: public UBaseUI {
	GENERATED_BODY()

public:
	ULOverlayUI();

	void ShowStatus(const bool Enabled) const;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SaveTimer();
	UFUNCTION()
	void SaveUpd(const bool NewIsSaving);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> SaveMat = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCurveFloat> SaveCurve = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> ISaving = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TTime = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TState = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UGroupBox> GStatus = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialInstanceDynamic> SaveMatDyn = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SaveTime = 1;
	
	bool IsSaving = false;
};
