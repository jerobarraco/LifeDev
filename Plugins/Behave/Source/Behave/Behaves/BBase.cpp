// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

#include "BConsts.h"

void UBBase::Begin_Implementation() {}
void UBBase::End_Implementation() {}
void UBBase::Tick_Implementation(float DT) {}
float UBBase::Has_Implementation(const FName& Token) { 
	float* const pVal = Values.Find(Token);
	return pVal ? *pVal : 0;
}

float UBBase::Want_Implementation(const FName& Token) { 
	return Has(Token);
}

float UBBase::Need_Implementation(const FName& Token) { return 0; }
EBDoRes UBBase::Do_Implementation(const float DT, FName& IOToken) { return EBDoRes::IGNORE; }
// void UBBase::Affect_Implementation(const FName& Token, const float Val) {
	// Values[Token] = FMath::Clamp(Values[Token]+Val, 0, 1);
// }

void UBBase::React_Implementation(const float DT, const FName& Token, const float Val) {}

#define BError .01


float UBBase::TopWant(FName& OToken) {
	// TODO at some point randomize, or have a range.
	// maybe add a random
	float VMax = -1;
	for (const FName& T: Tokens) {
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

float UBBase::TopNeed(FName& OToken) {
	float VMax = -1;
	for (const FName& T: Tokens) {
		const float VWant = Need(T) + FMath::RandRange(-BError, BError);;
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

	//Values[Token] =
	*pVal = FMath::Clamp((*pVal) + Dif, 0, 1);
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
