// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

#include "BConsts.h"
#include "JUtils/Misc/JUtilsMisc.h"

void UBBase::Begin_Implementation() {}

void UBBase::End_Implementation() {
	OnTrait.Clear();
	OnUpd.Clear();
}

void UBBase::Tick_Implementation(const float DT) {
	OnUpd.Broadcast(this);
}

float UBBase::Val_Implementation(const FName& Token) const {
	const float* const pVal = Values.Find(Token);
	return pVal ? *pVal : 0;
}

float UBBase::Want_Implementation(const FName& Token) {
	const float V = Val(Token);
	const FBWantNorm* const pNorm = WantNorms.Find(Token);
	
	if (!pNorm)
		return V>WantMin ? V: -1;

	const float VN = FMath::Clamp(UJUtilsMath::RemapNorm(V, pNorm->Thresh, 1), 0, 1);
	const float VV = V>pNorm->Thresh ? VN * pNorm->Target : -1;
	const float Error = FMath::RandRange(-pNorm->Error, pNorm->Error);
	const float VE = VV + Error;
	return VE;
}

EBDoRes UBBase::Do_Implementation(const float DT, FName& IOToken) { return EBDoRes::IGNORE; }
bool UBBase::ReactDo_Implementation(const float DT, const FName& Token) { return false; }
void UBBase::ReactState_Implementation(const float DT, const FName& Token, const float Val) {}

float UBBase::TopWant(FName& OToken) {
	float VMax = 0;
	TArray<FName> Wants;
	WantNorms.GetKeys(Wants);
	for (const FName& T: Wants) {
		// rand make it imperfect
		const float VWant = Want(T);
		
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

	*pVal = FMath::Clamp((*pVal) + Dif, 0, 1); //Values[Token] =
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
