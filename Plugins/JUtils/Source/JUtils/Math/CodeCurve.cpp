// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CodeCurve.h"

float UCodeCurveLib::LinCycle(float p){
	float p2 = FMath::Fmod(p,2.0f);
	if (p2>1)
		p2 = 2-p2;
	return p2;
}

float UCodeCurveLib::Rand(float p) {
	return FMath::FRand();
}

float UCodeCurveLib::Sin(float p) {
	return FMath::Sin(p*UE_PI);
}

float UCodeCurveLib::Cos(float p) {
	return FMath::Cos(p*UE_PI);
}

float UCodeCurveLib::SinFull(float p) {
	return FMath::Sin(p*UE_PI*2);
}

float UCodeCurveLib::CosFull(float p) {
	return FMath::Cos(p*UE_PI*2);
}


