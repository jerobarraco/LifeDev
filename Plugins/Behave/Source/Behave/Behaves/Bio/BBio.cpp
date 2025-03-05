// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/Emo/BEmo.h"

void UBBio::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);

	Values[T_Energy] -= BioDampE*DT;
	Values[T_Full] -= BioDampF*DT;
}

void UBBio::React_Implementation(const float DT, const FName& Token, const float Val) {
	Super::React_Implementation(DT, Token, Val);
	if (UNLIKELY(Token == UBEmo::T_Sad)) {
		const float Affect = Val > .8 ? BioDampE*-2 : 0;
		Mod(T_Energy, DT*Affect);
	} 
}
