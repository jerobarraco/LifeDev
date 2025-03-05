// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSpace.h"

#include "Behave/Behaves/BConsts.h"
#include "Behave/Behaves/Bio/BBio.h"

UBSpace::UBSpace():Super() {
	Tokens = {
	};

	Values = {
		// {T_Energy, 1},
		// {T_Full, 1},
		// {T_Warm, 1},
	};
}

void UBSpace::Tick_Implementation(const float DT) {
	Super::Tick_Implementation(DT);
	// Mod(T_Energy, -BioDampE*DT);
	// Mod(T_Full, -BioDampF*DT);
}

void UBSpace::React_Implementation(const float DT, const FName& Token, const float Val) {
	Super::React_Implementation(DT, Token, Val);
	// if (UNLIKELY(Token == UBEmo::T_Sad)) {
		// const float Affect = Val > .8 ? BioDampE*-1 : 0;
		// Mod(T_Energy, DT*Affect);
	// }
}

float UBSpace::Want_Implementation(const FName& Token) {
	return Super::Want_Implementation(Token);
	// return 1-Super::Want_Implementation(Token); // we want to max these vars
}

EBDoRes UBSpace::Do_Implementation(const FName& Token) {
	if (Token == UBBio::T_Hungry) {
		// TODO check surroundings to see if there's something edible.
		// stub: assume there isn't
		// TODO how to add a want?
		MoveTime = FMath::RandRange(1, 2);
		return EBDoRes::DO;
	}
	
	return EBDoRes::IGNORE;
}
