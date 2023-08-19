// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Interact/Animator/CAnimatorCam.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Chaps/All/Env/SRain.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALNPC01I00::ALNPC01I00():Super() {
	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);

	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Duration = 3;
	AnimFade->Meshes.Add(Mesh);
	AnimFade->Meshes.Add(Head);
	AnimFade->Meshes.Add(Torso);
	AnimFade->Meshes.Add(ArmL1);
	AnimFade->Meshes.Add(ArmL2);
	AnimFade->Meshes.Add(LegL1);
	AnimFade->Meshes.Add(LegL2);
	AnimFade->Meshes.Add(ArmR1);
	AnimFade->Meshes.Add(ArmR2);
	AnimFade->Meshes.Add(LegR1);
	AnimFade->Meshes.Add(LegR2);
	AnimFade->Meshes.Add(Pelvis);
	AnimFade->Meshes.Add(FootL);
	AnimFade->Meshes.Add(FootR);
}

void ALNPC01I00::BeginPlay() {
	Super::BeginPlay();
	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	AnimCam->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InOutCubic);
}

void ALNPC01I00::TriggerLocked_Implementation() {
	Dialogs->AddId("N01.L");
	Super::TriggerLocked_Implementation();
}

EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Card0) {
		Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagSitDone);
		Dialogs->AddId("N01.0");
		GetWorld()->GetSubsystem<UFlashback>()->SetVal(.8);
		return EItemUseResult::SUCCESS;
	}

	Dialogs->AddId("N01.IB");
	return EItemUseResult::BAD_HANDLED;
}

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0));
	AnimCam->Target = Head->GetComponentLocation();
	AnimCam->Play();
}

void ALNPC01I00::DiagSitDone() {
	Dialogs->OnDone.RemoveAll(this);
	
	StandUp();
	GetWorld()->GetSubsystem<UFlashback>()->SetVal(.4);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagStandDone);
	Dialogs->AddId("N01.1");
}

void ALNPC01I00::DiagStandDone() {
	Dialogs->OnDone.RemoveAll(this);

	if (IsValid(Card)) {
		Card->SetActorHiddenInGame(false);
	}

	ASRain* const R = Cast<ASRain>(UGameplayStatics::GetActorOfClass(GetWorld(), ASRain::StaticClass()));
	if (R) { R->SetPlaying(false); }

	GetWorld()->GetSubsystem<UFlashback>()->SetVal(.1);
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALNPC01I00::FadeDone);
	AnimFade->Play();
}

void ALNPC01I00::FadeDone() {
	Destroy();
}
