// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Kismet/GameplayStatics.h"

#include "Interact/Animator/CAnimatorCam.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Game/Chaps/All/Env/SRain.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALNPC01I00::ALNPC01I00():Super() {
	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);

	// AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Duration = 2;
	AnimFade->Meshes = {
		Mesh, Head, Torso, Pelvis,
		ArmL1, ArmL2, ArmR1, ArmR2,
		LegL1, LegL2, LegR1, LegR2,
		FootL, FootR
	};
}

void ALNPC01I00::BeginPlay() {
	Super::BeginPlay();
	Flashback = GetWorld()->GetSubsystem<UFlashback>();
	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	AnimCam->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InOutCubic);
}

void ALNPC01I00::TriggerLocked_Implementation() {
	Dialogs->AddId("N01.L");
	Super::TriggerLocked_Implementation();
}

EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
	if (Name != LDConsts::Items::Card0) {
		Dialogs->AddId("N01.IB");
		return EItemUseResult::BAD_HANDLED;
	}

	SetEnabled(false); // disable the interact so it can fade better, and player won´t trigger again
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagSitDone);
	Dialogs->AddId("N01.0");
	Flashback->SetVal(1);
	return EItemUseResult::SUCCESS;
}

void ALNPC01I00::DiagSitDone() {
	Dialogs->OnDone.RemoveAll(this);
	
	StandUp();
	Flashback->SetVal(.5);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagStandDone);
	Dialogs->AddId("N01.1");
}

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
	AnimCam->Target = Head->GetComponentLocation(); // look at character
	AnimCam->Play();
}

void ALNPC01I00::DiagStandDone() {
	Dialogs->OnDone.RemoveAll(this);

	if (IsValid(Card)) {
		Card->SetActorHiddenInGame(false);
	}

	UWorld* const W = GetWorld();
	ASRain::SetPlayingS(W, false);

	Flashback->SetVal(.2);
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALNPC01I00::FadeDone);
	AnimFade->Play();
}

void ALNPC01I00::FadeDone() {
	Destroy();
}
