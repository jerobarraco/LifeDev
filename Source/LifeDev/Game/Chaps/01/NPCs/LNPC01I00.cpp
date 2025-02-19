// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorCam.h"
#include "CQuickMesh.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"

ALNPC01I00::ALNPC01I00():Super() {
	LockedDlg = "N01.L";
	Texts = { FText::FromString("Looks dangerous...") };
	UseFade = true;
	UseStateLoop = false;

	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);
}

void ALNPC01I00::BeginPlay() {
	Super::BeginPlay();
	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	AnimCam->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InOutCubic);
}

void ALNPC01I00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
	if (Name != LDConsts::Items::Card0) {
		const bool Handled = Diags->AddId("N01.IB");
		return Handled ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	// disable the interact, so it can fade better, and player won't trigger again
	SetActive(false);
	Story->OnStart.AddUniqueDynamic(this, &ALNPC01I00::DoStepStart);
	Story->StartNext(); // trigger next step
	return EItemUseResult::SUCCESS;
}


void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
	AnimCam->Target = Head->GetComponentLocation(); // look at character
	AnimCam->Play();
}

void ALNPC01I00::DoStepStart(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	if (Step->Name == "C1S6") {
		StandUp();
	} else if (Step->Name == "C1S7") {
		Fade(false); // fade out manually. doRewards won't.
		DoRewards(); // give the card and disappear
	}
}

// TODO unbind from story
// TODO camera