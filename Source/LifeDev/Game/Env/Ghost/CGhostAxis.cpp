// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CGhostAxis.h"

#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CRandomizer.h"

UCGhostAxis::UCGhostAxis():Super() {
	Kp = 0.005;
	Ki = 0.005;
	Kd = 0.005;
	StopTime = -1;
	AutoUpdateValue = true;
	Super::SetAutoActivate(false);
	
	Anim = CreateDefaultSubobject<UCAnimator>(TEXT("Anim"));
	Anim->Duration = 10;
	Anim->IsLooping = true;
	Anim->IsBouncing = true;
	Anim->SetAutoActivate(false);
	Anim->Curve = nullptr;

	Rand = CreateDefaultSubobject<UCRandomizer>(TEXT("Rand"));
	Rand->DelayMin = 5;
	Rand->DelayMax = 10;
	Rand->ValueMin = 4;
	Rand->ValueMax = 8;
	Rand->IsLooping = true;
	Rand->UseAnimMirror = true;
	Rand->UseAnimValue = true;
	Rand->Anim = Anim;

	Anim->UseLog = false;
}

void UCGhostAxis::RenameComp(const FString& Postfix) {
	static const FString RandPre("Rand_");
	if (LIKELY(Rand)) Rand->Rename(*(RandPre+Postfix));

	static const FString AnimPre(TEXT("Anim_"));
	if (LIKELY(Anim)) Anim->Rename(*(AnimPre+Postfix));
}

void UCGhostAxis::Activate(const bool bReset) {
	Super::Activate(bReset);
	Rand->Activate(bReset);
	Anim->Activate(bReset);
}

void UCGhostAxis::Deactivate() {
	Super::Deactivate();
	Rand->Deactivate();
	Anim->Deactivate();
}

