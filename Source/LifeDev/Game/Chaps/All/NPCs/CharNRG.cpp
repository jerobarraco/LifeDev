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
// i'm just abusing the interface provided by the Interact, and the setup in the steps.

ACharNRG::ACharNRG():Super() {
	Texts = { FText::FromString("") };
	StateNum = 2;
	IsOneShot = false; // IsOneShot will call SetEnable as soon as it triggers.
	
	Anim->TRoot = Root; // nice try but... (read beginplay)
	Anim->IsAdditive = false;
	Interact->SetBoxExtent(FVector(.1)); // make it minimal. no need to interact with it.

	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(Mesh);
	Parts->bAutoManageAttachment = true;
	Parts->bAutoActivate = false;
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiag(TEXT("/Game/LifeDev/Game/Chars/CharEnergy_N"));
	Parts->SetAsset(CNiag.Object);
	
	// Super::SetEnabled_Implementation(false); // notice super and Implementation otherwise will call this function
	Super::SetEnabled(false); // The step will SetEnabled(true) via IntersFadeIn
}

void ACharNRG::BeginPlay() {
	Super::BeginPlay();
	Anim->TRoot = Root; // needed or it won't actually use it
}

void ACharNRG::SetEnabled(bool Enabled) {
	// // Super::SetEnabled_Implementation(Enabled); // we don't need the interact part
	if (Parts) Parts->SetActive(Enabled); // this is a bit of abuse, as enabled and showing !=
}

void ACharNRG::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	const static FName SSpawnRate("SpawnRate");
	const float Rate = State == 0 ? SpawnRateMax : SpawnRateMin;
	if (Parts) Parts->SetVariableFloat(SSpawnRate, Rate);
	// SetEnabled(false); // leave the parts active as i still want them to keep spawning
}
