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

float UCodeCurveLib::Bezier(float T, const TArray<float>& Ps) {
	const uint8 N = Ps.Num();
	if (N<2) return 0;

	TArray<float> NPS = Ps;
	for (uint8 i = 0; i<N-1; ++i) {
		for (uint8 j = 0; j<N-1-i; ++j) {
			NPS[j] = FMath::Lerp(NPS[j], NPS[j+1], T);
		}
	}
	return NPS[0];
}


/*
 * TODO port

def i_accel(p): return math.sin(math.pi*p*0.5)**2 #pi/2 = 90º
def i_deccel(p): return 1-i_accel(1-p)
def i_log(p):
	#be careful when using, numbers less than 1 gives error
	return math.log((p*2)+1)

#http://www.the-art-of-web.com/css/timing-function/ <<
#this is what I wanted to do with beizers a looooong time ago, but I couldn't
#same results, the idea is to do it with splines, it's not worth it though
def i_b_default(p):
	return PointBezier(p, 0, 0, 0.25, 0.1, 0.25, 1, 1, 1)[1]

def i_b_ease_in(p):
	return PointBezier(p, 0, 0, 0.42, 0.0, 1, 1, 1, 1)[1]

def i_b_ease_out(p):
	return PointBezier(p, 0, 0, 0, 0, 0.58, 1, 1, 1)[1]

def i_b_ease_in_out(p):
	return PointBezier(p, 0, 0, 0.42, 0.0, 0.58, 1, 1, 1)[1]

def i_b_cubic(p):
	return PointBezier(p, 0, 0, 0, 1.0, 1.0, 0, 1, 1)[1]

def i_b_backstart(p):
	return PointBezier(p, 0, 0,
		0.2, -0.3, 0.6, 0.26,
		1, 1)[1]

def i_b_boing(p):
	return PointBezier(p,
	0, 0,
	0.42, 0.0,
	0.58, 1.5,
	1, 1
	)[1]


def RanmaBezier(progress, points):
	"""
	Devuelve un punto (x, y) sobre una curva bezier dado el avance en la misma
	Admite curvas biezer de cualquier orden
	@progress como en interpolar, normalmente un numero entre 0 y 1 indicando el avance de sobre la curva
	@points : array de points -> [ [0, 0], [1, 1], [2, 2] ]
	es como PointBezier pero permite curvas de cualquier cantidad de points de control (de 1 a (teoricamente) infinito))
	es algo mas lento que PointBezier para curvas de la misma cantidad de points
	escrito por Ranma42 @ irc.freenode.net/#cairo
	"""

	while len(points)>1:
		points2 = []
		for i in range(len(points) - 1):
			px0,py0 = points[i]
			px1,py1 = points[i+1]
			p = (LERP(progress, px0, px1), LERP(progress, py0, py1))
			points2.append(p)
		points = points2
	return points[0]

def PointBezier(progress, x_start, y_start,  x1, y1, x2, y2, x_end, y_end):
	"""
	Devuelve un punto (x, y) sobre una curva bezier dado el avance en la misma
	@x_start, y_start : punto inicial de la curva
	@x1, y1 : 1º punto de control de la curva
	@x2, y2 : 2º punto de control de la curva
	@x_end, y_end : punto final de la curva
	@progress : avance sobre la curva (0 a 1)
	Esta funcion es igual que Bezier pero es algo más rápida, además,
	Está limitada a:
	1 Punto de inicio
	2 points de control
	1 Punto final
	y todos los points son pasados por parámetro secuencialmente.
	#with help of ranma42!
	"""

	curvx1 = LERP(progress, x_start, x1)
	curvx2 = LERP(progress, x1, x2)
	curvx3 = LERP(progress, x2, x_end)

	curvx4 = LERP(progress, curvx1, curvx2)
	curvx5 = LERP(progress, curvx2, curvx3)

	curvx6 = LERP(progress, curvx4, curvx5)

	curvy1 = LERP(progress, y_start, y1)
	curvy2 = LERP(progress, y1, y2)
	curvy3 = LERP(progress, y2, y_end)

	curvy4 = LERP(progress, curvy1, curvy2)
	curvy5 = LERP(progress, curvy2, curvy3)

	curvy6 = LERP(progress, curvy4, curvy5)
	return curvx6, curvy6
*/