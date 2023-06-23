// Copyright Jerónimo Barraco-Mármol

#include "DoorBase.h"

#include "Interact/CInteract.h"

ADoorBase::ADoorBase():Super() {

	Interact->SetRelativeLocation(FVector(-60.039127,-7.825052,100.782019));
	Interact->SetBoxExtent(FVector(60.000000,10.000000,103.743262));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);

	/*
	TODO reparent all the other stuff to doorlike and then add the frame here
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrame(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00-Frame.Door00-Frame"));
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);
	Frame->SetStaticMesh(CFrame.Object);
	Frame->SetRelativeRotation(FRotator(0.000000,180.000000,-0.000000));
	*/
}
