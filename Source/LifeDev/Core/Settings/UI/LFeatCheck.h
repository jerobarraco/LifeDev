// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

#include "LFeatCheck.generated.h"

class UTextBlock;
class ULSettings;

// a checkbox for a feature toggle
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API ULFeatCheck: public UUserWidget {
	GENERATED_BODY()

public:
	ULFeatCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetUp(EFeat NFeat, const FText& NewText);

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Load();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Apply();

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	
	UFUNCTION() // bind
	void FeatUpdate(EFeat Feat, bool bEnabled);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UCheckBox* Check = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	EFeat Feat = EFeat::NONE;
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettings* Settings = nullptr;
};
