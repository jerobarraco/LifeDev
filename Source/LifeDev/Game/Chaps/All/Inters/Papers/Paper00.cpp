// Copyright Jerónimo Barraco-Mármol

#include "Paper00.h"

#include "Interact/CInteract.h"

APaper00::APaper00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Paper00/Paper00.Paper00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.5,10.5,0));
	Interact->SetRelativeLocation(FVector(7.5,-10.5,0.25));
	Interact->SetBoxExtent(FVector(7.5,10.5,.250000));
	
	AnimEnabled = false;
	Texts = { FText::FromString(TEXT("A piece of paper")) } ;
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog
}
