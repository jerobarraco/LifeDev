// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LBLogW.h"

#include "Components/ScrollBox.h"

#include "Diags/Diags.h"

ULBLogW::ULBLogW():Super() {
	static ConstructorHelpers::FClassFinder<ULBLogItemW>
		CW(TEXT("/Game/LifeDev/Game/Dialogs/BLog/BLogItem_W.BLogItem_W_C"));
	ItemClass = CW.Succeeded() ? CW.Class.Get() : ULBLogItemW::StaticClass();
	static ConstructorHelpers::FClassFinder<ULBLogSpacerW>
		CSp(TEXT("/Game/LifeDev/Game/Dialogs/BLog/BLogSpacer_W.BLogSpacer_W_C"));
	SpacerClass = CSp.Succeeded() ? CSp.Class.Get() : ULBLogSpacerW::StaticClass();
}

void ULBLogW::NativeOnInitialized() {
	Super::NativeOnInitialized();

	UDiags* const Diags = UDiags::Instance(this);
	if (UNLIKELY(!Diags)) return;

	Diags->OnAdd.AddUniqueDynamic(this, &ULBLogW::DiagAdd);
	Diags->OnDone.AddUniqueDynamic(this, &ULBLogW::DiagSpace);
	// bind to the diags
}

void ULBLogW::NativeDestruct() {
	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void ULBLogW::DiagAdd(const FName Name, const FDialog& Diag) {
	if (UNLIKELY(Seen.Contains(Name))) return;
	Seen.Add(Name);

	if (UNLIKELY(!Scroller)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Need a scrollbox named Scroller."), __func__);
		return;
	}
	
	ULBLogItemW* const Widget = Cast<ULBLogItemW>(
		CreateWidget(this, ItemClass.Get()));
	if (UNLIKELY(!Widget)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Failed to create widget"), __func__);
		return;
	}

	Widget->SetUp(Diag);
	Scroller->AddChild(Widget);
}

void ULBLogW::DiagSpace() {
	if (UNLIKELY(!Scroller)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Need a scrollbox named Scroller."), __func__);
		return;
	}

	ULBLogSpacerW* const Widget = Cast<ULBLogSpacerW>(
		CreateWidget(this, SpacerClass.Get()));
	if (UNLIKELY(!Widget)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Failed to create widget"), __func__);
		return;
	}

	Scroller->AddChild(Widget);
}
