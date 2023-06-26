// Copyright Jerónimo Barraco-Mármol

#include "Door.h"

#include "Interact/CInteract.h"

ADoor::ADoor():Super() {

	Interact->SetRelativeLocation(FVector(-60.039127,-7.825052,100.782019));
	Interact->SetBoxExtent(FVector(60.000000,10.000000,103.743262));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);

	// TODO
	// static ConstructorHelpers::FObjectFinder<UStaticMesh>
		// CFrame(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	// Frame->SetStaticMesh(CFrame.Object);
}
