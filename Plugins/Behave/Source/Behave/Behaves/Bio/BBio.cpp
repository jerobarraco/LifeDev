// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBio.h"

#include "Behave/Behaves/Emo/BEmo.h"
#include "Behave/Behaves/Space/BSpace.h"

UBBio::UBBio():Super() {
	WantNorms = {
		{T_Tired, FBWantNorm{ .Thresh = .7, .Target = 1}},
		{T_Hungry, FBWantNorm{ .Thresh = .5, .Target = .6}},
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

bool UBBio::ReactDo_Implementation(const float DT, const FName& Token) {
	if (Token == UBSpace::T_Eat) {
		Mod(T_Hungry, -BioDampF*5*DT);
		Mod(T_Tired, BioDampE*DT);
		return Val(T_Hungry) <= .01 || Want(T_Tired) > .8;
	}

	if (Token == UBSpace::T_Sleep) {
		Mod(T_Tired, -BioDampE*20*DT);
		return Val(T_Tired) <= .1;
	}
	
	if (Token == UBSpace::T_Move){
		Mod(T_Tired, BioDampE*2*DT);
		return false; // move shouldn't be interrupted since it's instrumental to many things
	}

	if (Token == UBSpace::T_Play) {
		Mod(T_Tired, BioDampE*3*DT);
		return Want(T_Tired) > .8; // break if i'm really tired
	}

	return false;
}
