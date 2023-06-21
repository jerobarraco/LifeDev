// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"
#include "Niagara/Public/NiagaraComponent.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;

	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));

	Ghosts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ghosts"));
	Ghosts->SetupAttachment(Root);
	Ghosts->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CNiagara(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Ghost/Ghost_NS.Ghost_NS"));
	Ghosts->SetAsset(CNiagara.Object);
	Ghosts->SetRelativeLocation(FVector(207.355288,0.509086,48.526007));

	// static ConstructorHelpers::FClassFinder<UClass> CChar(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Ghost/Ghost_NS.Ghost_NS"));
	// CharClass = CChar.Class; 	
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	SpawnGhosts();
}

void ALStepC1S002::SpawnGhosts() const {
	Ghosts->Activate(true);
	// Ghosts->ResetSystem();
}

void ALStepC1S002::SpawnChar() const {
	if (CharClass) {
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor(CharClass, &CharT, Params);
		
	}
}

