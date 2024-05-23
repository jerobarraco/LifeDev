// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MsgBox.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

#include "JButton.h"

void UMsgBox::NativeOnInitialized() {
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed); // start collapsed

	const TArray<UJButton*> UBtns = {Btn0, Btn1, Btn2};
	const int32 Num = UBtns.Num();
	for (int32 i=0; i<Num; ++i) {
		UJButton* const B = UBtns[i];
		if (!B) continue;
		B->OnClick.AddUniqueDynamic(this, &UMsgBox::BtnClick);
	}

	OnHideFinished.BindDynamic(this, &UMsgBox::HideAnimFinish);
}

void UMsgBox::NativeDestruct() {
	for (UJButton* const B: {Btn0, Btn1, Btn2}) {
		if (!B) continue;
		B->OnClick.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UMsgBox::Init(const FText& Message, const TArray<FText>& Texts) {
	Msg->SetText(Message);

	const TArray<UJButton*> UBtns = {Btn0, Btn1, Btn2};
	const int32 Num = Texts.Num();
	const int32 Num2 = UBtns.Num();
	
	for (int32 i=0; i<Num2; ++i) {
		UJButton* const B = UBtns[i];
		if (!B) continue;

		const FText& T = i < Num ? Texts[i] : FText();
		const bool Show = i<Num && !T.IsEmpty();
		B->SetVisibility( Show ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (!Show) continue;
		B->SetUp(Texts[i], i);
	}
}

void UMsgBox::Show_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	Super::Show_Implementation(); // will already make visible.
	if (!AnimShow) return;

	const float Speed = UKismetMathLibrary::SafeDivide(1.0, AnimDuration);
	PlayAnimation(AnimShow, 0, 1,
			EUMGSequencePlayMode::Forward, Speed);
}

void UMsgBox::HideAnimFinish() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// this func is needed so that we can call the base version after the anim finishes.
	// i could bind to super. but then i would need to make it ufunction and not.

	// important to unbind, otherwise it will be called on the next show. Y_Y
	UnbindFromAnimationFinished(AnimShow, OnHideFinished);
	Super::Hide_Implementation(); // this will already collapse.
}

void UMsgBox::Hide_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	if (!AnimShow || FMath::IsNearlyZero(AnimDuration)) {
		HideAnimFinish();
		return;
	}

	const float Speed = UKismetMathLibrary::SafeDivide(1.0, AnimDuration);
	PlayAnimation(AnimShow, 0, 1,
		EUMGSequencePlayMode::Reverse, Speed);

	// timer might not work here. who knew.
	// important to rebind on each call, see note on hideanimfinished.
	BindToAnimationFinished(AnimShow, OnHideFinished);
}

void UMsgBox::BtnClick(const int32 ID) {
	UE_LOG(LogTemp, Log, TEXT("Btn click id=%i"), ID);
	Done(ID);
}
