// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorCam.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

// i've considered moving stuff from here to steps. but they would be a bunch of steps.
// and naming them would be difficult. so no.

ALNPC01I00::ALNPC01I00():Super() {
	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);
	LockedDlg = "N01.L";
	Texts = { FText::FromString("He seems dangerous...") };
	UseFade = true;
}

void ALNPC01I00::BeginPlay() {
	Super::BeginPlay();
	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	AnimCam->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InOutCubic);
}

EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
	if (Name != LDConsts::Items::Card0) {
		Diags->AddId("N01.IB");
		return EItemUseResult::BAD_HANDLED;
	}

	// disable the interact, so it can fade better, and player won't trigger again
	SetActive(false);
	Diags->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagSitDone);
	Diags->AddId("N01.0");
	Flashback->SetVal(1);
	ALMusicMan::SetRainS(GetWorld(), true);
	
	return EItemUseResult::SUCCESS;
}

void ALNPC01I00::DiagSitDone() {
	Diags->OnDone.RemoveAll(this);
	
	Flashback->SetVal(.5);
	Diags->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagStandDone);
	Diags->AddId("N01.1");

	StandUp();
}

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
	AnimCam->Target = Head->GetComponentLocation(); // look at character
	AnimCam->Play();
}

void ALNPC01I00::DiagStandDone() {
	Diags->OnDone.RemoveAll(this);

	UWorld* const W = GetWorld();
	ALMusicMan::SetRainS(W, false);
	Flashback->SetVal(.2);

	Fade(false); // fade out manually. doRewards won't.
	DoRewards(); // give the card and disappear
}
