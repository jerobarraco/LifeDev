// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"

#include "Components/HorizontalBox.h"

#include "LifeDev/Core/Settings/LSysSettings.h"

#include "LFeatGroup.generated.h"

class ULFeatCheck;
class UTextBlock;
class ULSettings;

// a checkbox for a feature toggle
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULFeatGroup: public UHorizontalBox {
	GENERATED_BODY()

public:
	ULFeatGroup(const FObjectInitializer& O);

	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetUp(const TMap<EFeat, FText>& InFeats);

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Load();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Apply();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Reset();

protected:

	void Clear();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULFeatCheck> CheckClass = nullptr;
	
	TMap<EFeat, TObjectPtr<ULFeatCheck>> Feats;
};
