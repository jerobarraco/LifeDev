// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Scissor00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

AScissor00::AScissor00() {
	Texts = { NSLOCTEXT("Scissor", "State0", "Scissors") };
	StateNum = 1;
	UseAnim = false;
	UseFade = true;

	// TODO fix material, it shows black on runtime (probably the animfade)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Utils/Scissor_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-4.500000,0.,-1.));
	Mesh->SetCastAllShadows(true);
	AnimFade->MatBase = nullptr; // fix material

	Interact->SetRelativeLocation(FVector(4.500000,-12.500000,1.000000));
	Interact->SetBoxExtent(FVector(4.500000,12.500000,1.));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Utils/Scissor/scissor_cut"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndB(TEXT("/Game/LifeDev/Game/Inters/Utils/Scissor/Scissors_16_Slow_Snip"));
	SFXs = { CSnd.Object, CSndB.Object };
	
	ALInteract::SetMobility(EComponentMobility::Static);
}
