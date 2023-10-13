// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S001.h"

#include "Kismet/GameplayStatics.h"

ALStepC1S001::ALStepC1S001():Super() {
	Name = FName("C1S1");
	InputEnabled = true;
	CamTarget = nullptr;
	UsePawnCam = true;
	// finish on obtaining these.
	FinishItems = {
		"T0",
		"WM"
	};
}

void ALStepC1S001::Start_Implementation() {
	Super::Start_Implementation();
}

void ALStepC1S001::Debug_Implementation() {
	Super::Debug_Implementation();
	AActor* const Pawn = UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass());
	if (Pawn) {
		Pawn->SetActorLocation(FVector(-57.163605,815.932276,79.606879));
	}
}

