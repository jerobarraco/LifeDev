// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "BaseUI.h"

#include "JUtils/JMiscUtils.h"

void UBaseUI::Show_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	SetVisibility(ESlateVisibility::Visible);
	if (ShowCursor)
		UJMiscUtils::ShowUI(this, true, this, true);
}

void UBaseUI::Hide_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// can put Collapsed into a var in case i ever want to change that on children
	SetVisibility(ESlateVisibility::Collapsed);
	if (ShowCursor) 
		UJMiscUtils::ShowUI(this, false, nullptr, true);

	if (AutoUnbind) {
		OnDone.Clear();
		OnDoneVal.Clear();
	}
}

void UBaseUI::Done(int32 RetVal) {
	OnDone.Broadcast();
	OnDoneVal.Broadcast(RetVal);
}
