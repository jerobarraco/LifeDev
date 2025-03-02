// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "BaseUI.h"

#include "JUtils/Misc/JUtilsMisc.h"

void UBaseUI::Show_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	SetVisibility(ESlateVisibility::Visible);
	if (ShowCursor)
		UJUtilsMisc::ShowUI(this, true, this, true);
}

void UBaseUI::Hide_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// can put Collapsed into a var in case i ever want to change that on children
	SetVisibility(ESlateVisibility::Collapsed);
	if (ShowCursor)
		UJUtilsMisc::ShowUI(this, false, nullptr, true);

	if (AutoUnbind) {
		OnDone.Clear();
		OnDoneVal.Clear();
	}
}

void UBaseUI::Done(const int32 RetVal) {
	OnDone.Broadcast();
	OnDoneVal.Broadcast(RetVal);
	Hide();
}
