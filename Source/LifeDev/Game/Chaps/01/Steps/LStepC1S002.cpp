// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"

#include "Camera/CameraComponent.h"

#include "Diags/Diags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;
	UseFBDlgAuto = false; // will be manually set.
	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));

	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);
	GhostPos = FVector(200,-4,-75);
	UseGhosts = true;
	ItemsRem = { LDConsts::Items::Tape1 }; // use up the item
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	
	FB->SetMax(1); // reset to 1 since we will change it several times here
	static const FName DId("C1S2.0"); // done only for the autodiagFb. notice that after Super::Start it has no effect.
	FBDlgAutoTo = 1;
	SetFBDlgAuto(DId); // will calculate based on the actual dialog group.
	// FB->SetVal(.75); // was already clamped to .7 on c1s0, so it can't be bigger
	// can't use autodiagfb since the logic is somewhat more complex
	// FBDlgMod = (1.0 - FB->GetValTo()) / 4.0;

	Diags->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StartShake);
	Diags->AddId(DId); // "i'll use the tape"
}

void ALStepC1S002::StartShake() {
	Diags->OnDone.RemoveDynamic(this, &ALStepC1S002::StartShake);
	// don't affect further.
	FBDlgMod = 0;
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	CamShakeStart();
	// FB->SetVal(1); // bump to max

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ALStepC1S002::ShakeStarted, 2);
}

void ALStepC1S002::ShakeStarted() {
	Diags->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StopShake);
	Diags->AddId("C1S2.1"); // "it got worse"
}

void ALStepC1S002::StopShake() {
	Diags->OnDone.RemoveDynamic(this, &ALStepC1S002::StopShake);
	CamShakeStop();
	FB->SetVal(.85);

	Finish();
}

// TODO this would work better if i split this into two steps. so i can use fbdiag auto and cam shake easily.