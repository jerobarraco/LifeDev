// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"
#include "Niagara/Public/NiagaraComponent.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;

	Ghosts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ghosts"));
	Ghosts->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CNiagara(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Ghost/Ghost_NS.Ghost_NS"));
	Ghosts->SetAutoActivate(false);
	Ghosts->SetAsset(CNiagara.Object);
	Ghosts->SetRelativeLocation(FVector(207.355288,0.509086,48.526007));
	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	SpawnGhosts();
}

void ALStepC1S002::SpawnGhosts() {
	
	Ghosts->Activate(true);
	// Ghosts->ResetSystem();	
}

