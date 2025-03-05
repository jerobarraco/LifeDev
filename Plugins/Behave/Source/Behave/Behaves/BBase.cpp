// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

void UBBase::Init_Implementation() {}
void UBBase::Tick_Implementation(float DT) {}
float UBBase::Has_Implementation(const FName& Token) { return 0; }
float UBBase::Want_Implementation(const FName& Token) { return 0; }
float UBBase::Need_Implementation(const FName& Token) { return 0; }
void UBBase::Do_Implementation(const FName& Token) {}

float UBBase::TopWant(FName& OToken) {
	float VMax = -1;
	for (const FName& T: Tokens) {
		const float VWant = Want(T);
		if (VWant <= VMax) continue;
		OToken = T;
		VMax = VWant;
	}

	return VMax;
}

float UBBase::TopNeed(FName& OToken) {
	float VMax = -1;
	for (const FName& T: Tokens) {
		const float VWant = Need(T);
		if (VWant <= VMax) continue;
		OToken = T;
		VMax = VWant;
	}

	return VMax;
}