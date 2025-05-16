// Copyright Jerónimo Barraco-Mármol

#include "Batts.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ABatts::ABatts():Super() {
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	UseAutoActivate = true;
	UseRewardDestroy = true;
	RewardFlash = .1;
	RewardItem = LDConsts::Items::Batts;
	Texts = { NSLOCTEXT("Batts", "State0", "Batteries") } ;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Batts00/Batts00.Batts00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.5,2,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,90));
	Mesh->SetCastAllShadows(true);
	
	MeshB = CreateDefaultSubobject<UCQuickMesh>(TEXT("MeshB"));
	MeshB->SetupAttachment(Mesh);
	MeshB->SetStaticMesh(CMesh.Object);
	MeshB->SetRelativeLocation(FVector(-0.116369,0.143369,-2.243960));
	MeshB->SetRelativeRotation(FRotator(-11,0,0));
	MeshB->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(2.522632,-1.030120,-0.728246));
	Interact->SetBoxExtent(FVector(4.2,0.7,2.5));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C.Grab_C"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	/// Anims
	AnimFade->Meshes.AddUnique(MeshB);
	UseFade = true; // a bit lazy. ideally to be set on the children.

	// set static by default
	ABatts::SetMobility(EComponentMobility::Static);
}

void ABatts::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	MeshB->SetMobility(Mobility);
}
