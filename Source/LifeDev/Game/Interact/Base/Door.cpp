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
	Frame->SetComponentTickEnabled(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrameMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00-Frame.Door00-Frame"));
	Frame->SetStaticMesh(CFrameMesh.Object);
	Frame->SetRelativeLocation(FVector(0,-10,0));
	Frame->SetRelativeRotation(FRotator(0,180,0).Quaternion());
	
	// TODO
	// static ConstructorHelpers::FObjectFinder<UStaticMesh>
		// CFrame(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	// Frame->SetStaticMesh(CFrame.Object);
}
