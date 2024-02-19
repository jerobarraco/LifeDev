// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true; // important since the door, also due to the load
	FinishPostWait = false;
	TeleportChar = true;
	ItemsRem = {"T01"};
	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);

	GhostPos = FVector(210,-42,-65);
	UseGhosts = true;


	// This is kinda lame. but since the next chapter is so close, and we use a fade,
	// i think i can get away with it.
	// TODO add the objects i need to the other layer for chap00
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Chaps/All/Datalayers/Chap01_DL.Chap01_DL"));
	if (CDL1.Succeeded()) {
		DL_Load.Add(CDL1.Object);
	}
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();

	UWorld* const W = GetWorld();
	FB->SetMax(1);
	FB->SetVal(1, 10);

	ALMusicMan::SetRainS(W, true);
}

void ALStepC0S001::Stop_Implementation() {
	Super::Stop_Implementation();
	UWorld* const W = GetWorld();

	ALMusicMan::SetRainS(W, false);
	FB->SetVal(0);
}

void ALStepC0S001::TeleportPlayer() {
	// TODO use TeleportChar on the next step and use the step position in the world instead :V
	if (!IsValid(PlayerPos)) {
		UE_LOG(LogTemp, Log, TEXT("Player pos not set on C0S001"));
		return;
	}

	ALChar* const Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(GetWorld(), ALChar::StaticClass()));
	if (!Char) return;
	
	Char->TeleportTo(PlayerPos->GetActorLocation(), PlayerPos->GetActorRotation());
}
