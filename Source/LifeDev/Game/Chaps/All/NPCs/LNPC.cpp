// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC.h"

#include "Interact/Animator/CAnimatorMix.h"

ALNPC::ALNPC():Super() {
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ALNPC::SetFadeMat(UMaterialInterface* NewMat) {
	FadeMatClass = NewMat ? NewMat : FadeMatClass;
	FadeMat = Mesh->CreateDynamicMaterialInstance(0, FadeMatClass);
	Anim->Mat = FadeMat;
	Anim->MatFEnd = 1;
	Anim->MatFStart = 0;
	Anim->MatFName = "Opacity";
}

void ALNPC::Fade(bool In) {
	const bool Rev = !In;
	Anim->Play(Rev);
}

