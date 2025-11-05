// Copyright Jerónimo Barraco-Mármol

#include "Sponge.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASponge::ASponge():Super() {
	Texts = { NSLOCTEXT("Sponge", "State0", "Sponge") };
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Sponge"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(10,-5,5));
	Interact->SetBoxExtent(FVector(15,10,10));
	SFX->SetRelativeLocation(FVector(10,-5,5));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02.Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03.Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));

	// SFX_Start = { CSndClose.Object, CSndOpen.Object };
	// SFX_Trigger = CSndClose.Object;
	// TODO sound
}

