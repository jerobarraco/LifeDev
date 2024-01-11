// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

ALStepEnd::ALStepEnd():Super() {
	Name = FName("End");
	static FText ST = FText::FromString("~ To be continued ... ~");
	Title = ST;
	InputEnabled = false;
	UseFadeTime = true;
	FinishPostWait = false; // avoid crash
	UseFade = true;
	// don't change the camera
	UsePawnCam = true;
	CamTarget = nullptr;
}

void ALStepEnd::OpenLevel() {
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true);
}

void ALStepEnd::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetMin(0);
	FB->SetVal(0);
	
	FTimerHandle H;
	UWorld* const World = GetWorld();
	if (!World) return;
	World->GetTimerManager().SetTimer(H, this, &ALStepEnd::OpenLevel, WaitTime);

	ALMusicMan::FadeS(World, false);
}
