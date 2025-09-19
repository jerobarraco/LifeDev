// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "BaseUI.h"

#include "JUtilsUI.h"

void UBaseUI::Show_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	SetVisibility(ESlateVisibility::Visible);
	if (ShowCursor)
		UJUtilsUI::ShowUI(this, true, this);
}

void UBaseUI::Hide_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// can put Collapsed into a var in case i ever want to change that on children
	SetVisibility(ESlateVisibility::Collapsed);
	if (ShowCursor)
		UJUtilsUI::ShowUI(this, false, nullptr);

	if (AutoUnbind) {
		OnDone.Clear();
		OnDoneVal.Clear();
	}
}

void UBaseUI::Done(const int32 RetVal) {
	OnDone.Broadcast();
	OnDoneVal.Broadcast(RetVal);
}
