// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsGroup.h"

#include "LFeatCheck.h"

ULFeatsGroup::ULFeatsGroup(const FObjectInitializer& O):Super(O) {
	CheckClass = ULFeatCheck::StaticClass();
}

void ULFeatsGroup::SetUp(const TMap<EFeat, FText>& InTexts) {
	UE_LOG(LogTemp, Log, TEXT("LFeatGroup::%hs feat n=%i"), __func__, InTexts.Num());
	Texts = InTexts;
	
	// unfortunately this can't be called on SetUp, since that SetUp is called during onInitialize of the parent
	// which makes the feats crash...
	FeatsCreate();
}

void ULFeatsGroup::FeatsCreate() {
	FeatsClear();

	for (const TTuple<EFeat, FText>& F: Texts) {
		ULFeatCheck* const C = NewObject<ULFeatCheck>(this, CheckClass);
		if (!IsValid(C)) continue;
		AddChild(C); // maybe this fixes the crash
		C->SetUp(F.Key, F.Value);
	}
}

void ULFeatsGroup::Load() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Load();
	}
}

void ULFeatsGroup::Apply() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Apply();
	}
}

void ULFeatsGroup::Reset() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Reset();
	}
}

void ULFeatsGroup::FeatsClear() {
	ClearChildren();
	Feats.Empty(); // widgets can't be manually destroyed. yikes
}
