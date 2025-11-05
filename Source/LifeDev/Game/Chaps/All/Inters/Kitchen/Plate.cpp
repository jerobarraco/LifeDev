// Copyright Jerónimo Barraco-Mármol

#include "Plate.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APlate::APlate():Super() {
	Texts = { NSLOCTEXT("Plate", "State0", "Plate")};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseFade = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Plate/Plate00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(0,0,0));
	Interact->SetBoxExtent(FVector(20,20,5));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02.Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03.Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));

	// clearing the base mat on the anim fade.
	// not using SetNewMaterial since this mesh uses palettes, hence it will be overriden on the level outliner.
	// still doesn't seem to work.
	AnimFade->MatBase = nullptr;
}
