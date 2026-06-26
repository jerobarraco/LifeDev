// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

// some of these curves are taken from here https://easings.net/en#
#include "CodeCurve.h"

#include "Animation/AnimCurveTypes.h"

UCodeCurveLib* UCodeCurveLib::Instance() {
	return GetMutableDefault<UCodeCurveLib>();
}

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

float UCodeCurveLib::UOut(float p) {
	return FMath::InterpEaseOut(0, 1, p, 1);
}

float UCodeCurveLib::UIn(float p) {
	return FMath::InterpEaseIn(0, 1, p, 1);
}

float UCodeCurveLib::UInOut(float p) {
	return FMath::InterpEaseInOut(0, 1, p, 1);
}

float UCodeCurveLib::UStep(float p) {
	return FMath::InterpStep(0, 1, p, 10);
}

float UCodeCurveLib::USinIn(float p) {
	return FMath::InterpSinIn(0, 1, p);
}

float UCodeCurveLib::USinOut(float p) {
	return FMath::InterpSinOut(0, 1, p);
}

float UCodeCurveLib::USinInOut(float p) {
	return FMath::InterpSinInOut(0, 1, p);
}

float UCodeCurveLib::UCircularIn(float p) {
	return FMath::InterpCircularIn(0,1,p);
}

float UCodeCurveLib::UCircularOut(float p) {
	return FMath::InterpCircularOut(0,1,p);
}
float UCodeCurveLib::UCircularInOut(float p) {
	return FMath::InterpCircularInOut(0,1,p);
}

float UCodeCurveLib::InSin(const float p) {
	// taken from InterpSinIn. similar to easings https://easings.net/#easeInSine
	return -1.f * FMath::Cos(p * UE_HALF_PI) + 1.f;
}

float UCodeCurveLib::OutBounce(float p) {
	// https://easings.net/#easeOutBounce
	constexpr float n1 = 7.5625;
	constexpr float d1 = 2.75;

	if (p < 1 / d1) {
		return n1 * p * p;
	}
	if (p < 2 / d1) {
		const float p1 = p-1.5;
		return n1 * (p/ d1) * p1 + 0.75;
	}
	if (p < 2.5 / d1) {
		const float p2 = p-2.25;
		return n1 * (p / d1) * p2 + 0.9375;
	} 
	const float p3 = p-2.625;
	return n1 * (p / d1) * p3 + 0.984375;
}

float UCodeCurveLib::InBounce(float p) {
	return 1.f - OutBounce(1.f - p);
}

float UCodeCurveLib::InOutBounce(float p) {
	return p < 0.5f
		? (1.f - OutBounce(1.f - 2.f * p)) / 2.f
		: (1.f + OutBounce(2.f * p - 1.f)) / 2.f;
}

float UCodeCurveLib::InOutCubic(float p) {
	return p  < 0.5 ? 4.f * p  * p  * p  : 1.f - FMath::Pow(-2.f * p  + 2.f, 3) / 2.0;
}

float UCodeCurveLib::InCubic(float p) {
	return p*p*p;
}

float UCodeCurveLib::OutCubic(float p) {
	return 1.f - FMath::Pow(1.0f - p, 3.0f);
}

float UCodeCurveLib::InOutQuart(const float p) {
	return p < 0.5 ?
		2 * p * p :
		1 - FMath::Pow((-2 * p) + 2, 2) / 2.0;
}

float UCodeCurveLib::InBack(float p) {
	constexpr float c1 = 1.70158;
	constexpr float c3 = c1 + 1;
	const float pp = p*p;
	return (c3 * pp * p) - (c1 * pp);
}

float UCodeCurveLib::OutBack(float p) {
	// https://easings.net/#easeOutBack
	constexpr float c1 = 1.70158;
	constexpr float c3 = c1 + 1.0;

	const float pn = p - 1;
	const float ppn = pn*pn;
	return 1.0 + (c3 * (ppn*pn)) + (c1 * ppn);
}

float UCodeCurveLib::InOutBack(float p) {
	constexpr float c1 = 1.70158;
	constexpr float c2 = c1 * 1.525;
	constexpr float c21 = (c2 + 1);

	const float p2 = 2 * p;
	const float pm2 = p2 - 2;
	
	return p < 0.5
		? (p2*p2 * ((c21 * p2) - c2)) / 2.0f
		: (pm2*pm2 * ((c21 * pm2) + c2) + 2.) / 2.f;
}

float UCodeCurveLib::InExpo(float p) {
	// 2^10*p-10
	return FMath::IsNearlyZero(p) ? 0 : FMath::Pow(2, 10 * p - 10);
}

float UCodeCurveLib::OutExpo(float p) {
	return FMath::IsNearlyEqual(p, 1) ? 1 : 1 - FMath::Pow(2, -10 * p);
}

float UCodeCurveLib::InOutExpo(float p) {
	return FMath::IsNearlyZero(p) ? 0 :
		( FMath::IsNearlyEqual(p, 1) ? 1 :
		( p < 0.5 ?
			FMath::Pow(2, 20 * p - 10) / 2 :
			(2 - FMath::Pow(2, -20 * p + 10)) / 2));
}


float UCodeCurveLib::BOutInQuad(float T) {
	return UE::Curves::BezierInterp(0.0, .75, .25, 1.0, T);
}

// TODO implement the rest

FFloatCurve UCodeCurveLib::GetBEaseIn() {
	FFloatCurve F;

	// 0, 0,  0.42, 0.0,  0.58, 1,  1, 1
	F.UpdateOrAddKey(0, 0);
	F.UpdateOrAddKey(0, .42);
	F.UpdateOrAddKey(1, .58);
	F.UpdateOrAddKey(1, 1);
	// FKeyHandle Handle = F.FloatCurve.AddKey(0, 0, false);
	return MoveTempIfPossible(F);
}

float UCodeCurveLib::DualBezier(float T, const TArray<float> Py, const TArray<float> Px) {
	const float T2 = Bezier(T, Px);
	return Bezier(T2, Py);
}

float UCodeCurveLib::Bezier(float T, const TArray<float>& Ps) {
	const uint8 N = Ps.Num();
	if (N<2) return 0;

	TArray<float> NPS = Ps;
	for (uint8 i = 0; i<N-1; ++i) {
		for (uint8 j = 0; j<N-1-i; ++j) {
			NPS[j] = FMath::LerpStable(NPS[j], NPS[j+1], T);
		}
	}
	return NPS[0];
}


// https://easings.net/#
// https://doc.qt.io/qt-6/qeasingcurve.html
// http://www.the-art-of-web.com/css/timing-function/


/*
 Kinda works
UCurveFloat* CurveFloat = NewObject<UCurveFloat>();
FRichCurve& F = CurveFloat->FloatCurve;

// 0, 0,  0.42, 0.0,  0.58, 1,  1, 1
F.UpdateOrAddKey(0, 0);
F.UpdateOrAddKey(0, .42);
F.UpdateOrAddKey(1, .58);
F.UpdateOrAddKey(1, 1); 
F.AutoSetTangents();
Anim->Curve = CurveFloat;

*/