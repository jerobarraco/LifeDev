// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

#include "LFeatCheck.generated.h"

class UTextBlock;
class ULSettings;

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API ULFeatCheck: public UUserWidget {
	GENERATED_BODY()

public:
	ULFeatCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat);

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void CheckUpdate(bool bIsChecked);
	UFUNCTION()
	void FeatUpdate(EFeat Feat, bool bEnabled);
	
	UPROPERTY(BlueprintReadOnly)
	EFeat Feat = EFeat::NONE;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UCheckBox* Check = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Text = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettings* Settings = nullptr;
};
