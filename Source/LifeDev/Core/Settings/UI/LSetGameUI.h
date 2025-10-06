// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetGameUI.generated.h"

enum class EFeat : uint8;
class UComboBoxString;
class ULFeatsGroup;
class UTextBlock;
class USlider;

// base class for the game settings ui
UCLASS(Blueprintable)
class LIFEDEV_API ULSetGameUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void ClearConfig();

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void DiagAutoTimeUpd(const float Value);
	UFUNCTION()
	void FBTimeUpd(const float Value);
	UFUNCTION()
	void InterDragUpd(const float Value);
	UFUNCTION()
	void InterHintUpd(const float Value);
	UFUNCTION()
	void BlurSizeUpd(const float Value);
	UFUNCTION()
	void FringeUpd(const float Value);

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatsGroup> FeatsGroup;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLDiagAutoTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TDiagAutoTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLFBTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TFBTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLInterDrag;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TInterDrag;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLInterHint;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TInterHint;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLBlurSize;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TBlurSize;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLFringe;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TFringe;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UComboBoxString> Langs;
};
