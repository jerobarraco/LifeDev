// Copyright Jerónimo Barraco-Mármol

#include "CharNRG.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"

// q = there are 2 ways. either i do it the "right" way or i cheat really badly.
// p = i only have 40 minutes.
// res = i cheat really hard

ACharNRG::ACharNRG():Super() {
	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(Mesh);
	Parts->bAutoManageAttachment = true;
	Parts->bAutoActivate = false;
	Texts = { FText::FromString("") };
	StateNum = 2;
	// IsOneShot = true; // nopes it will call setenable as soon as it triggers.
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiag(TEXT("/Game/LifeDev/Game/Chars/CharEnergy_N"));
	Parts->SetAsset(CNiag.Object);
	Super::SetEnabled(false); // notice super
}

void ACharNRG::SetEnabled_Implementation(bool Enabled) {
	// // i don't really want this to be interactable though
	// i'm just abusing the interface provided by the interact, and the setup in the steps.
	// Super::SetEnabled_Implementation(Enabled);
	Parts->SetActive(Enabled); // this is a bit of abuse, as enabled and showing !=
}

void ACharNRG::AnimEnd_Implementation() {
	Super::AnimEnd_Implementation();
	SetEnabled(false); // TODO change param for spawn count
}
