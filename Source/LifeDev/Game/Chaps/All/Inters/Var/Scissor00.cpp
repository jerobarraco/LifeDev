// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Scissor00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

AScissor00::AScissor00() {
	Texts = { NSLOCTEXT("Scissor", "State0", "Scissors") };
	StateNum = 1;
	UseAnim = false;
	UseFade = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Pencil_P"));
	Mesh->SetStaticMesh(CMesh.Object);

	Interact->SetBoxExtent(FVector(7.5,1,1));
	Interact->SetRelativeLocation(FVector(7,-0.750000,0.750000));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC"));
	// SFXTrigger = CSnd.Object;
	SFXs = {  }; // TODO
	
	ALInteract::SetMobility(EComponentMobility::Static);
}
