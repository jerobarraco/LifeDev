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
class LIFEDEV_API ULFeatCheck: public UUserWidget {
	GENERATED_BODY()

public:
	ULFeatCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetUp(const EFeat NFeat, const FText& NewText);

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Load();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Apply();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Reset();

	// if true it will call apply on change.
	// you can call Reset to re-apply the value from the last Load (or SetUp)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool AutoApply = false;

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void CheckChanged(const bool bIsChecked);
	
	UFUNCTION() // bind
	void FeatUpdate(const EFeat Feat, const bool bEnabled);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UCheckBox* Check = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Text = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	EFeat Feat = EFeat::NONE;
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettings* Settings = nullptr;

private:
	bool WasEnabled = false;
};
