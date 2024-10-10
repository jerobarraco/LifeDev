// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatGroup.h"

#include "LFeatCheck.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULFeatGroup::ULFeatGroup(const FObjectInitializer& O):Super(O) {
	CheckClass = ULFeatCheck::StaticClass();
}

void ULFeatGroup::SetUp(const TMap<EFeat, FText>& InFeats) {
	UE_LOG(LogTemp, Log, TEXT("%hs feat =%i"), __func__, InFeats.Num());
	Clear();

	for (const TTuple<EFeat, FText>& F: InFeats) {
		ULFeatCheck* const C = NewObject<ULFeatCheck>(this, CheckClass);
		if (!IsValid(C)) continue;
		C->SetUp(F.Key, F.Value);
		AddChild(C);
	}
	
	Load();
}

void ULFeatGroup::Load() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Load();
	}
}

void ULFeatGroup::Apply() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Apply();
	}
}

void ULFeatGroup::Reset() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (!IsValid(Check)) continue;
		Check->Reset();
	}
}

void ULFeatGroup::Clear() {
	ClearChildren();
	Feats.Empty(); // widgets can't be manually destroyed. yikes
}
