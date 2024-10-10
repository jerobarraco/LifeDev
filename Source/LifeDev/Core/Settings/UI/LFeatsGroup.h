// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"

#include "Components/HorizontalBox.h"

#include "LifeDev/Core/Settings/LSysSettings.h"

#include "LFeatsGroup.generated.h"

class ULFeatCheck;
class UTextBlock;
class ULSettings;

// a checkbox for a feature toggle
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULFeatsGroup: public UHorizontalBox {
	GENERATED_BODY()

public:
	ULFeatsGroup(const FObjectInitializer& O);

	// don't call this on onInitialize of your widget. it will nastly crash.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetUp(const TMap<EFeat, FText>& InTexts);

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Load();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Apply();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(UnsafeDuringActorConstruction))
	void Reset();

protected:
	void FeatsClear();
	void FeatsCreate();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULFeatCheck> CheckClass = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TMap<EFeat, TObjectPtr<ULFeatCheck>> Feats;
	
	TMap<EFeat, FText> Texts;
};
