// Copyright Jerónimo Barraco-Mármol

#include "Door.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ADoor::ADoor():Super() {
	Interact->SetRelativeLocation(FVector(-60.039127,-7.825052,100.782019));
	Interact->SetBoxExtent(FVector(60.000000,10.000000,103.743262));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);
	// this is to block the user for trespassing
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	
	Frame = CreateDefaultSubobject<UCQuickMesh>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrameMesh(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/Door00-Frame.Door00-Frame"));
	Frame->SetStaticMesh(CFrameMesh.Object);
	Frame->SetRelativeLocation(FVector(0,-10,0));
	Frame->SetRelativeRotation(FRotator(0,180,0).Quaternion());
	Frame->SetCastShadow(true);
	Frame->bCastDynamicShadow = true;

	// Frame->SetMobility(EComponentMobility::Static);
	// can't make static component that is child of movable
}
