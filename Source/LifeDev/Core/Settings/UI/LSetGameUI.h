// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetGameUI.generated.h"

class UTextBlock;
class USlider;
// base class for the game settings ui
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetGameUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

protected:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void DiagAutoTimeUpd(const float Value);
	UFUNCTION()
	void FBTimeUpd(const float Value);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLDiagAutoTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TDiagAutoTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<USlider> SLFBTime;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TFBTime;
};
