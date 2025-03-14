// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

bool UBBase::CanDoSelf_Implementation() const {
	return true;
}

bool UBBase::CanDo_Implementation() const {
	return CanDoSelf();
}

EBDoRes UBBase::Do_Implementation(const float DT) {
	return EBDoRes::STOP;
}

void UBBase::SetState_Implementation(const EBState New) {
	State = New;
}
