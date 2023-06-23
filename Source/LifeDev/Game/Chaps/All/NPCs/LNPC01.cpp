// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01.h"

#include "Interact/CInteract.h"

ALNPC01::ALNPC01():Super() {
	TriggerDlg = "N01";
	AnimEnabled = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Char18.Char18"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-30.000000,30.000000,10.092946));

	Interact->SetRelativeLocation(FVector(32.500000,-30.000000,59.988557));
	Interact->SetBoxExtent(FVector(20.000000,20.000000,70.000000));
}

void ALNPC01::DiagDone() {
	Destroy();
	Dialogs->OnDone.RemoveAll(this);
	Inventory->Mod("C1", 1);
}

void ALNPC01::Trigger_Implementation() {
	Super::Trigger_Implementation();
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01::DiagDone);
	Dialogs->AddId("N01.0");
	
	// TODO add dialog for join the party
	// and reward card
	// TODO reward new card. 
	// TODO finish the chapter after that
}
