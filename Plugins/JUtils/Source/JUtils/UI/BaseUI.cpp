// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "BaseUI.h"

#include "JUtils/JMiscUtils.h"

void UBaseUI::Show_Implementation() {
	if (ShowCursor) {
		UJMiscUtils::ShowUI(true, GetWorld(), this, true);
	}
}

void UBaseUI::Hide_Implementation() {
	if (ShowCursor) {
		UJMiscUtils::ShowUI(false, GetWorld(), nullptr, true);
	}
}

void UBaseUI::Done(int32 RetVal) {
	OnDone.Broadcast();
	OnDoneVal.Broadcast(RetVal);
}
