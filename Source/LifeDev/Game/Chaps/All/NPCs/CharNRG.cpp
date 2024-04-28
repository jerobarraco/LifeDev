// Copyright Jerónimo Barraco-Mármol

#include "CharNRG.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"

ACharNRG::ACharNRG():Super() {
	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(Mesh);
	Parts->bAutoManageAttachment = true;
	Parts->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiag(TEXT("/Game/LifeDev/Game/Chars/CharEnergy_N"));
	Parts->SetAsset(CNiag.Object);
}

void ACharNRG::SetEnabled_Implementation(bool Enabled) {
	Super::SetEnabled_Implementation(Enabled);
	Parts->SetActive(Enabled); // this is a bit of abuse, as enabled and showing !=
}
