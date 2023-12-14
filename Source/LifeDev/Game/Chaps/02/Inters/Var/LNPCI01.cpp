// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI01.h"

#include "Interact/Animator/CAnimatorCam.h"

ALNPCI01::ALNPCI01():Super() {
	AnimCam = CreateDefaultSubobject<UCAnimatorCam>(TEXT("AnimCam"));
	AnimCam->Duration = 2;
	AnimCam->SetComponentTickInterval(1/60.f);
}

void ALNPCI01::BeginPlay() {
	Super::BeginPlay();
	UCodeCurveLib* const Lib = NewObject<UCodeCurveLib>();
	AnimCam->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::InOutCubic);
}
