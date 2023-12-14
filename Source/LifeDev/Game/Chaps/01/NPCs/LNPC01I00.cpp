// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorCam.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALNPC01I00::ALNPC01I00():Super() {
	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);
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
	if (Name != LDConsts::Items::Card0) {
		Dialogs->AddId("N01.IB");
		return EItemUseResult::BAD_HANDLED;
	}

	SetEnabled(false); // disable the interact so it can fade better, and player won't trigger again
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagSitDone);
	Dialogs->AddId("N01.0");
	Flashback->SetVal(1);
	
	return EItemUseResult::SUCCESS;
}

void ALNPC01I00::DiagSitDone() {
	Dialogs->OnDone.RemoveAll(this);
	
	Flashback->SetVal(.5);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagStandDone);
	Dialogs->AddId("N01.1");

	StandUp();
}

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
	AnimCam->Target = Head->GetComponentLocation(); // look at character
	AnimCam->Play();
}

void ALNPC01I00::DiagStandDone() {
	Dialogs->OnDone.RemoveAll(this);

	UWorld* const W = GetWorld();
	ALMusicMan::SetRainS(W, false);
	Flashback->SetVal(.2);

	DoRewards(); // give the card and disappear
}
