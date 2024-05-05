// Copyright Jerónimo Barraco-Mármol

#include "CharNRG.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

// q = there are 2 ways. either i do it the "right" way or i cheat really badly.
// p = i only have 40 minutes.
// res = i cheat really hard


// i don't really want this to be interactable though
// i'm just abusing the interface provided by the interact, and the setup in the steps.

ACharNRG::ACharNRG():Super() {
	Texts = { FText::FromString("") };
	StateNum = 2;
	// IsOneShot = true; // nopes it will call setenable as soon as it triggers.
	
	
	Anim->TRoot = Root; // nice try but... (read beginplay)
	Anim->IsAdditive = false;
	Interact->SetBoxExtent(FVector(.1)); // make it minimal. no need to interact with it.
	
	Super::SetEnabled(false); // notice super

	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(Mesh);
	Parts->bAutoManageAttachment = true;
	Parts->bAutoActivate = false;
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiag(TEXT("/Game/LifeDev/Game/Chars/CharEnergy_N"));
	Parts->SetAsset(CNiag.Object);
}

void ACharNRG::BeginPlay() {
	Super::BeginPlay();
	Anim->TRoot = Root; // needed or it won't actually use it
}

void ACharNRG::SetEnabled_Implementation(bool Enabled) {
	// Super::SetEnabled_Implementation(Enabled);
	Parts->SetActive(Enabled); // this is a bit of abuse, as enabled and showing !=
}

void ACharNRG::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	const static FName SSpawnRate("SpawnRate");
	const float Rate = State == 0 ? SpawnRateMax : SpawnRateMin;
	Parts->SetVariableFloat(SSpawnRate, Rate);
	// leave the parts active as i still want them to keep spawning
	// SetEnabled(false); // TODO change param for spawn count
}
