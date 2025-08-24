// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LFeatsGroup.h"

#include "Components/ScrollBox.h"

#include "LFeatCheck.h"

ULFeatsGroup::ULFeatsGroup(const FObjectInitializer& O):Super(O) {
	SetOrientation(Orient_Horizontal);
	
	static ConstructorHelpers::FClassFinder<ULFeatCheck>
		CCheck(TEXT("/Game/LifeDev/Core/Settings/Feats/FeatCheck_W"));
	CheckClass = CCheck.Succeeded() ? CCheck.Class.Get() : ULFeatCheck::StaticClass();
	
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSB(TEXT("/Game/LifeDev/Core/UI/LScrollBar_S"));
	StyleScroll = CSB.Object;
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
		ULFeatCheck* const C = CreateWidget<ULFeatCheck>(this, CheckClass);
		if (UNLIKELY(!IsValid(C))) continue;

		AddChild(C); // maybe this fixes the crash
		C->SetPadding(ChildPadding);
		C->SetUp(F.Key, F.Value);
		Feats.Add(F.Key, C);
	}
}

void ULFeatsGroup::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}

void ULFeatsGroup::Load() {
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (UNLIKELY(!IsValid(Check))) continue;
		Check->Load();
	}
}

void ULFeatsGroup::Apply() {
	UE_LOG(LogTemp, Log, TEXT("FeatsGroup::%hs apply num =%i"), __func__, Feats.Num());
	for (const TTuple<EFeat, TObjectPtr<ULFeatCheck>>& F: Feats) {
		const TObjectPtr<ULFeatCheck>& Check = F.Value;
		if (UNLIKELY(!IsValid(Check.Get()))) continue;
		UE_LOG(LogTemp, Log, TEXT("FeatsGroup::%hs apply check=%s"), __func__, *GetNameSafe(Check));
		Check->Apply();
	}
}

void ULFeatsGroup::ResetStyle() {
	if (StyleScroll) {
		const FScrollBarStyle* const S = StyleScroll->GetStyle<FScrollBarStyle>();
		if (LIKELY(S)) {
			SetWidgetBarStyle(*S);
			SetScrollbarThickness(FVector2D(S->Thickness));
		}
	}
}

void ULFeatsGroup::FeatsClear() {
	ClearChildren();
	Feats.Empty(); // widgets can't be manually destroyed. yikes
}
