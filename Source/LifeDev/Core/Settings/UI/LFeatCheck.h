// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CoreMinimal.h"
#include "Base/LCheck.h"

#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LFeatCheck.generated.h"

class UCheckBox;
class UTextBlock;
class ULSettings;

// a checkbox for a feature toggle
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULFeatCheck: public ULCheck {
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

	UPROPERTY(BlueprintReadOnly, Transient)
	EFeat Feat = EFeat::NONE;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;

private:
	bool WasEnabled = false;
};
