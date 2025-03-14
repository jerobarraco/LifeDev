// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSeq.h"

EBDoRes UBSeq::Do_Implementation(const float DT) {
	UBBase* const C = GetCurChild();
	if (UNLIKELY(!C)) return EBDoRes::ABORT; // anomaly

	const EBDoRes R = C->Do(DT);
	if (R == EBDoRes::ABORT) return EBDoRes::ABORT; // bubble up

	if (R == EBDoRes::STOP) {
		StartChild(CurChildI+1);
		if (CurChildI<0)
			return EBDoRes::STOP;
	}
}
