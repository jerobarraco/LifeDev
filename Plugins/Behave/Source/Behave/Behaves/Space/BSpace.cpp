// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSpace.h"

#include "Behave/Behaves/BConsts.h"
#include "Behave/Behaves/Bio/BBio.h"
#include "Behave/Behaves/Emo/BEmo.h"

UBSpace::UBSpace():Super() {
	Wants = {};

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

void UBSpace::ReactState_Implementation(const float DT, const FName& Token, const float Val) {
	Super::ReactState_Implementation(DT, Token, Val);
	// if (UNLIKELY(Token == UBEmo::T_Sad)) {
		// const float Affect = Val > .8 ? BioDampE*-1 : 0;
		// Mod(T_Energy, DT*Affect);
	// }
}

float UBSpace::Want_Implementation(const FName& Token) {
	return Super::Want_Implementation(Token);
	// return 1-Super::Want_Implementation(Token); // we want to max these vars
}

EBDoRes UBSpace::Do_Implementation(const float DT, FName& IOToken) {
	if (IOToken == UBBio::T_Hungry) {
		// add a new token. i'm unsure about this. it will decouple the "want hungry" from the "action/want eat"
		// with the idea that there are many ways to do something.
		IOToken = T_Eat;
		return EBDoRes::NEW;
	} else if (IOToken == T_Eat) {
		// not sure about this. it seems using the tokens for wants and actions is making it very hard to track. TODO reconsider
		if (Moved) {
			EatTime -= DT;
			if (UNLIKELY(EatTime<=0)) {
				Moved = false;
				return EBDoRes::FINISH;
			}
			return EBDoRes::DO;
		}
		// TODO check surroundings to see if there's something edible.
		// stub: assume there isn't

		Moved = true; // micro opt, no need to set on each tick of TMOVE
		MoveTime = FMath::RandRange(3, 5);
		EatTime = FMath::RandRange(5, 10); // micro opt again
		IOToken = T_Move; // issue a new want
		return EBDoRes::NEW; // will continue.
	} else if (IOToken == T_Move) {
		MoveTime -= DT;
		return MoveTime <=0 ? EBDoRes::FINISH : EBDoRes::DO;
	} else if (IOToken == UBBio::T_Tired) {
		IOToken = T_Sleep;
		return EBDoRes::NEW;
	} else if (IOToken == T_Sleep) {
		if (Moved) {
			SleepTime -= DT;
			if (UNLIKELY(SleepTime<=0)) {
				Moved = false;
				return EBDoRes::FINISH;
			}
			return EBDoRes::DO;
		}

		Moved = true; // micro opt, no need to set on each tick of TMOVE
		SleepTime = FMath::RandRange(5, 10); // micro opt again
		MoveTime = FMath::RandRange(3, 5);
		IOToken = T_Move; // issue a new want
		return EBDoRes::NEW; // will continue.
	} else if (IOToken == UBEmo::T_Bore) {
		IOToken = T_Play;
		return EBDoRes::NEW;
	} else if (IOToken == T_Play) {
		if (Moved) {
			PlayTime -= DT;
			if (UNLIKELY(PlayTime<=0)) {
				Moved = false;
				return EBDoRes::FINISH;
			}
			return EBDoRes::DO;
		}

		Moved = true; // micro opt, no need to set on each tick of TMOVE
		PlayTime = FMath::RandRange(5, 10); // micro opt again
		MoveTime = FMath::RandRange(3, 5);
		IOToken = T_Move; // issue a new want
		return EBDoRes::NEW; // will continue.
	}

	return EBDoRes::IGNORE;
}
