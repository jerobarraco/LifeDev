// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "MsgBox.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

#include "JButton.h"
#include "Components/HorizontalBox.h"

UMsgBox::UMsgBox():Super() {
	static ConstructorHelpers::FClassFinder<UJButton>
		CBtn(TEXT("/JUtils/UI/JButton_W.JButton_W_C"));
	BtnClass = CBtn.Class;
}

void UMsgBox::NativeOnInitialized() {
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed); // start collapsed
	OnHideFinished.BindDynamic(this, &UMsgBox::HideAnimFinish);
}

void UMsgBox::NativeDestruct() {
	BtnsClear();
	Super::NativeDestruct();
}

void UMsgBox::Bind() {
	for (UJButton* const B: Btns) {
		if (UNLIKELY(!B)) continue;
		B->OnClick.AddUniqueDynamic(this, &UMsgBox::BtnClick);
	}
}

void UMsgBox::Unbind() {
	for (UJButton* const B: Btns) {
		if (UNLIKELY(!B)) continue;
		B->OnClick.RemoveAll(this);
	}
}

void UMsgBox::BtnsClear(const uint32 Reserve) {
	Unbind();
	BtnBox->ClearChildren();
	Btns.Empty(Reserve);
}

void UMsgBox::SetUp(const FText& Message, const TArray<FText>& Texts) {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	Msg->SetText(Message);
	
	const int32 Num = Texts.Num();
	BtnsClear(Num);

	if (UNLIKELY(!BtnBox)) return;

	for (int32 i=0; i<Num; ++i) {
		UJButton* const B = Cast<UJButton>(CreateWidget(this, BtnClass.Get()));
		if (UNLIKELY(!B)) continue;

		B->SetUp(Texts[i], i);
		B->SetPadding(FMargin(Pad, 0,0,0));
		Btns.AddUnique(B);
		BtnBox->AddChild(B);
	}
}

void UMsgBox::Show_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	Super::Show_Implementation(); // will already make visible.
	if (UNLIKELY(!AnimShow)) return;

	const float Speed = UKismetMathLibrary::SafeDivide(1.0, AnimDuration);
	PlayAnimation(AnimShow, 0, 1,
		EUMGSequencePlayMode::Forward, Speed);
	Bind(); // probably un/binding on show/hide since the buttons are not guaranteed before/after this.
}

void UMsgBox::HideAnimFinish() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// this func is needed so that we can call the base version after the anim finishes.
	// i could bind to super. but then i would need to make it ufunction and not.

	// important to unbind, otherwise it will be called on the next show. Y_Y
	UnbindFromAnimationFinished(AnimShow, OnHideFinished);
	Super::Hide_Implementation(); // this will already collapse and unbind.
	OnHidden.Broadcast();
	OnHidden.Clear();
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
	Unbind(); // no double clicks here
	Done(ID);
	Hide(); // hide after done because of autounbind
}
