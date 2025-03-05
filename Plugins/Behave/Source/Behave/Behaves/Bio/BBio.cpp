// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/BConsts.h"
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

void UBBio::ReactState_Implementation(const float DT, const FName& Token, const float Val) {
	Super::ReactState_Implementation(DT, Token, Val);
	if (UNLIKELY(Token == UBEmo::T_Sad)) {
		const float Affect = Val > .8 ? BioDampE*-1 : 0;
		Mod(T_Tired, DT*Affect);
	} 
}

void UBBio::ReactDo_Implementation(const float DT, const FName& Token) {
	if (Token != T_Hungry) return;
	Mod(T_Hungry, -BioDampE*5*DT);
}

EBDoRes UBBio::Do_Implementation(const float DT, FName& IOToken) {
	if (IOToken == T_Hungry && Val(T_Hungry) < .15) return EBDoRes::FINISH; // race condition with space
	return EBDoRes::IGNORE;
}
