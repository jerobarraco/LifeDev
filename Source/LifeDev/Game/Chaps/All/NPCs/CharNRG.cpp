// Copyright Jerónimo Barraco-Mármol

#include "CharNRG.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

// it's not that necessary to be an interact. but being so helps a ton.

ACharNRG::ACharNRG():Super() {
	Texts = { NSLOCTEXT("CharNrg", "State0", "") };
	StateNum = 2;
	IsOneShot = true; // IsOneShot will call SetEnable as soon as it triggers.
	UseAutoActivate = false;
	SetActorHiddenInGame(true); // start hidden by default

	Anim->TRoot = Root; // nice try but... (read beginplay)
	// the TEnd is set on the editor.
	Anim->IsAdditive = false;
	Interact->SetBoxExtent(FVector(.1)); // make it minimal. no need to interact with it.

	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(Mesh);
	Parts->bAutoManageAttachment = true;
	Parts->bAutoActivate = false;
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiag(TEXT("/Game/LifeDev/Game/Chars/CharEnergy_N"));
	Parts->SetAsset(CNiag.Object);
	// the part color is set on the editor (todo move here?)
}

void ACharNRG::BeginPlay() {
	Super::BeginPlay();
	Anim->TRoot = Root; // needed or it won't actually use it (why?)
	Anim->CopyTStart();
}

void ACharNRG::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	const static FName SSpawnRate("SpawnRate");
	const float Rate = State == 0 ? SpawnRateMax : SpawnRateMin;
	if (LIKELY(Parts)) Parts->SetVariableFloat(SSpawnRate, Rate);
	// SetEnabled(false); // leave the parts active as i still want them to keep spawning
}

void ACharNRG::SetActorHiddenInGame(const bool NewHidden) {
	Super::SetActorHiddenInGame(NewHidden);
	if (LIKELY(Parts)) Parts->SetActive(!NewHidden);
	//beware interact has a member called emitter
}
