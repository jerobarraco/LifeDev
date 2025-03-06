// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/BConsts.h"
#include "Behave/Behaves/Emo/BEmo.h"
#include "Behave/Behaves/Space/BSpace.h"

UBBio::UBBio():Super() {
	Wants = {
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

EBDoRes UBBio::ReactDo_Implementation(const float DT, FName& IOToken) {
	if (IOToken == UBSpace::T_Eat) {
		Mod(T_Hungry, -BioDampF*5*DT);
		Mod(T_Tired, BioDampE*DT);
		return Val(T_Hungry) <= .01 ? EBDoRes::FINISH: EBDoRes::IGNORE;
	}
	
	if (IOToken == UBSpace::T_Sleep) {
		Mod(T_Tired, -BioDampE*5*DT);
		return Val(T_Tired) <= .01 ? EBDoRes::FINISH: EBDoRes::IGNORE;
	}
	
	if (IOToken == UBSpace::T_Move || IOToken == UBSpace::T_Play) {
		Mod(T_Tired, BioDampE*2*DT);
		return EBDoRes::IGNORE;
	}

	return EBDoRes::IGNORE;
}
