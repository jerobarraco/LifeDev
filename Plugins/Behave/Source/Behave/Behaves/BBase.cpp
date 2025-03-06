// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

#include "BConsts.h"

#define BError .01

void UBBase::Begin_Implementation() {}

void UBBase::End_Implementation() {}

void UBBase::Tick_Implementation(float DT) {}

float UBBase::Val_Implementation(const FName& Token) { 
	float* const pVal = Values.Find(Token);
	return pVal ? *pVal : 0;
}

float UBBase::Want_Implementation(const FName& Token) {
	const float V = Val(Token);
	return V>WantMin ? V: -1;
}

EBDoRes UBBase::Do_Implementation(const float DT, FName& IOToken) { return EBDoRes::IGNORE; }
void UBBase::ReactState_Implementation(const float DT, const FName& Token, const float Val) {}
void UBBase::ReactDo_Implementation(const float DT, const FName& Token) { }

float UBBase::TopWant(FName& OToken) {
	// TODO at some point randomize, or have a range.
	// maybe add a random
	float VMax = 0;
	for (const FName& T: Wants) {
		// rand make it imperfect
		const float VWant = Want(T) + FMath::RandRange(-BError, BError);
		
		if (VWant <= VMax) continue;
		OToken = T;
		VMax = VWant;
	}
	UE_LOG(LogTemp, Log, TEXT("BBase:%hs Want=%s V=%.5f"),
		__func__, *OToken.ToString(), VMax);
	return VMax;
}

void UBBase::Mod(const FName& Token, const float Dif) {
	float* const pVal = Values.Find(Token);
	if (UNLIKELY(!pVal)) return;
	
	*pVal = FMath::Clamp((*pVal) + Dif, 0, 1); 	//Values[Token] =
}

void UBBase::Dump_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("%hs %s"),
			__func__, *GetNameSafe(this));
	for(TTuple<FName, float> KV: Values) {
		UE_LOG(LogTemp, Log, TEXT("%hs %s K=%s V=%.5f"),
			__func__, *GetNameSafe(this), *KV.Key.ToString(), KV.Value);
	}
}

// https://www.tomlooman.com/unreal-engine-asset-manager-async-loading/
