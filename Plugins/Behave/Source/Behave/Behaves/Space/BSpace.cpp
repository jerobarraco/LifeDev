// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BSpace.h"

#include "Behave/Behaves/BConsts.h"
#include "Behave/Behaves/Bio/BBio.h"
#include "Behave/Behaves/Emo/BEmo.h"

UBSpace::UBSpace():Super() {
	Values = {
		// {T_Energy, 1},
		// {T_Full, 1},
		// {T_Warm, 1},
	};
}

void UBSpace::MoveStop() {
	Moved = true;
}

void UBSpace::End_Implementation() {
	OnFoodClose.Clear();
	OnMoveToFood.Clear();
	Super::End_Implementation();
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
	return -1; // wants nothing
	// return Super::Want_Implementation(Token);
	// return 1-Super::Want_Implementation(Token); // we want to max these vars
}

EBDoRes UBSpace::Do_Implementation(const float DT, FName& IOToken) {
	if (IOToken == UBBio::T_Hungry) {
		// add a new token. i'm unsure about this. it will decouple the "want hungry" from the "action/want eat"
		// with the idea that there are many ways to do something.
		IOToken = T_Eat;
		return EBDoRes::NEW;
	} else if (IOToken == T_Eat) {
		const bool Can = OnFoodClose.IsBound() ? OnFoodClose.Execute() : false; // todo do something
		if (Can) return EBDoRes::DO;

		Moved = false;
		// this might be triggering movetofood on call to Do, which is inefficient // TODO
		OnMoveToFood.ExecuteIfBound();
		OnMoveTo.ExecuteIfBound(T_Eat);
		IOToken = T_Move; // issue a new want
		return EBDoRes::NEW; // will continue.
	} else if (IOToken == T_Move) {
		return Moved ? EBDoRes::FINISH : EBDoRes::DO;
	} else if (IOToken == UBBio::T_Tired) {
		IOToken = T_Sleep;
		return EBDoRes::NEW;
	} else if (IOToken == T_Sleep) {
		if (!MovedSleep) {
			MovedSleep = true; // todo sleepclose
			MovedPlay = false;
			
			Moved = false;
			OnMoveToSleep.ExecuteIfBound();
			OnMoveTo.ExecuteIfBound(T_Sleep);
			IOToken = T_Move; // issue a new want
			return EBDoRes::NEW; // will continue.
		}
		return EBDoRes::DO;
	} else if (IOToken == UBEmo::T_Bore) {
		IOToken = T_Play;
		return EBDoRes::NEW;
	} else if (IOToken == T_Play) {
		if (!MovedPlay) {
			MovedPlay = true;
			MovedSleep = false;
			Moved = false;
			OnMoveToPlay.ExecuteIfBound();
			OnMoveTo.ExecuteIfBound(T_Play);
			IOToken = T_Move; // issue a new want
			return EBDoRes::NEW; // will continue
		}
		return EBDoRes::DO;
	}

	return EBDoRes::IGNORE;
}
