// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "BaseUI.h"

#include "LOverlayUI.generated.h"

class UMaterialInstance;
class UImage;

// a ui root for things that show in an overlay. like the save icon.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API ULOverlayUI: public UBaseUI {
	GENERATED_BODY()

public:
	ULOverlayUI();

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SaveTimer();
	UFUNCTION()
	void SaveUpd(const bool NewIsSaving);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float SaveTime = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> SaveMat = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCurveFloat> SaveCurve = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> ISaving = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialInstanceDynamic> SaveMatDyn = nullptr;
	
	bool IsSaving = false;
};
