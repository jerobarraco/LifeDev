// Copyright Jerónimo Barraco-Mármol

#include "Plate.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APlate::APlate():Super() {
	Texts = { FText::FromString(TEXT("Plate"))};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Plate/Plate00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(0,0,0));
	Interact->SetBoxExtent(FVector(20,20,5));
	SFX->SetRelativeLocation(FVector(0,0,0));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02.Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03.Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));

	SetEnabled(false);
	// clearing the base mat on the anim fade.
	// not using SetNewMaterial since this mesh uses palettes, hence it will be overriden on the level outliner.
	// still doesn't seem to work.
	AnimFade->MatBase = nullptr;
	// SFX_Start = { CSndClose.Object, CSndOpen.Object };
	// SFX_Trigger = CSndClose.Object;
	// TODO sound
}

void APlate::BeginPlay() {
	Super::BeginPlay();
	// forced, to be able to use fade, relies on animfade->matbase being null
	// this is inefficient since i'm creating a new material instance and maybe i don't need it
	// but makes the code clearer later.
	AnimFade->CreateMaterial();
}

