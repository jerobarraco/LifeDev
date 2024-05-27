// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S001.h"

#include "Kismet/GameplayStatics.h"

ALStepC1S001::ALStepC1S001():Super() {
	Name = FName("C1S1");
	InputEnabled = true;
	CamTarget = nullptr;
	// no need to set it again since it's already set.
	// maybe that triggers another blend
	UsePawnCam = false;

	// finish on obtaining these.
	ItemsFinish = { "T00", "WM" };
}

void ALStepC1S001::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	AActor* const Pawn = UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass());
	if (Pawn) Pawn->SetActorLocation(FVector(-57.163605,815.932276,79.606879));
}

