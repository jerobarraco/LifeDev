// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01.h"

ALNPC01::ALNPC01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Char18.Char18"));
	Mesh->SetStaticMesh(CMesh.Object);
	TriggerDlg = "N01";
}

void ALNPC01::DiagDone() {
	Destroy();
	GetWorld()->GetSubsystem<UDialogs>()->OnDone.RemoveAll(this);
	 
}

void ALNPC01::Trigger_Implementation() {
	Super::Trigger_Implementation();
	GetWorld()->GetSubsystem<UDialogs>()->OnDone.AddUniqueDynamic(this, &ALNPC01::DiagDone);
	// TODO add dialog for join the party
	// and reward card
	// TODO reward new card. 
	// TODO finish the chapter after that
}
