// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFadeTime = true;
	FinishPostWait = false;
	UsePawnCam = true;

	// TODO can make this its own component type subchild of UCRandomizer
	RandFB = CreateDefaultSubobject<UCRandomizer>(TEXT("RandFlashback"));
	RandFB->DelayMin = .5;
	RandFB->DelayMax = 4;
	RandFB->ValueMin = -.05;
	RandFB->ValueMax = .5;
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();

	UFlashback::SetValS(GetWorld(), 0.1);
	RandFB->OnTriggerVal.AddUniqueDynamic(this, &ALStepC0S000::TriggerFB);
	RandFB->Activate(true);
}

void ALStepC0S000::Stop_Implementation() {
	RandFB->OnTriggerVal.RemoveAll(this);
	RandFB->Deactivate();
	Super::Stop_Implementation();
	
	// UFlashback::SetValS(GetWorld(), 0, 5);
}

void ALStepC0S000::BeginPlay() {
	Super::BeginPlay();
}

void ALStepC0S000::TriggerFB(float Val) {
	GetWorld()->GetSubsystem<UFlashback>()->IncVal(Val);
}
