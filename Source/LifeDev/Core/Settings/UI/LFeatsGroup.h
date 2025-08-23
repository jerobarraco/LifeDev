// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CoreMinimal.h"

#include "Components/ScrollBox.h"

#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LFeatsGroup.generated.h"

class UScrollBox;
class ULFeatCheck;
class UTextBlock;
class ULSettings;

// a checkbox for a feature toggle
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULFeatsGroup: public UScrollBox {
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
	UFUNCTION(BlueprintCallable, CallInEditor, Category=SetUp)
	void ResetStyle();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FMargin ChildPadding = FMargin(10.000000,10.000000,0.000000,10.000000);

protected:
	void FeatsClear();
	void FeatsCreate();
	virtual void OnWidgetRebuilt() override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULFeatCheck> CheckClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<USlateWidgetStyleAsset> StyleScroll = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TMap<EFeat, TObjectPtr<ULFeatCheck>> Feats;

	TMap<EFeat, FText> Texts;
};
