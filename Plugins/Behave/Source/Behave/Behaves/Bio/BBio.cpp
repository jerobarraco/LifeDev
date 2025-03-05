// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/Emo/BEmo.h"

UBBio::UBBio():Super() {
	Tokens = {
		T_Energy, T_Warm, T_Full
	};

	Values = {
		{T_Energy, 1},
		{T_Full, 1},
		{T_Warm, 1},
	};
}

void UBBio::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);
	Mod(T_Energy, -BioDampE*DT);
	Mod(T_Full, -BioDampF*DT);
}

void UBBio::React_Implementation(const float DT, const FName& Token, const float Val) {
	Super::React_Implementation(DT, Token, Val);
	if (UNLIKELY(Token == UBEmo::T_Sad)) {
		const float Affect = Val > .8 ? BioDampE*-1 : 0;
		Mod(T_Energy, DT*Affect);
	} 
}

float UBBio::Want_Implementation(const FName& Token) {
	return 1-Super::Want_Implementation(Token); // we want to max these vars
}
