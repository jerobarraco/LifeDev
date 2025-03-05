// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/Emo/BEmo.h"

UBBio::UBBio():Super() {
	Tokens = {
		T_Tired, T_Cold, T_Hungry
	};

	Values = {
		{T_Tired, 0},
		{T_Hungry, 0},
		{T_Cold, 0},
	};
}

void UBBio::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);
	Mod(T_Tired, BioDampE*DT);
	Mod(T_Hungry, BioDampF*DT);
}

void UBBio::React_Implementation(const float DT, const FName& Token, const float Val) {
	Super::React_Implementation(DT, Token, Val);
	if (UNLIKELY(Token == UBEmo::T_Sad)) {
		const float Affect = Val > .8 ? BioDampE*-1 : 0;
		Mod(T_Tired, DT*Affect);
	} 
}
