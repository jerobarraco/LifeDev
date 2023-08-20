// Copyright Jerónimo Barraco-Mármol

#include "Clout04.h"

#include "Interact/CInteract.h"

AClout04::AClout04():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Sock00.Sock00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	ItemReward = TEXT("C1C04"); // chapter 1 clout 4
	TriggerDlg = FName("C04");
	Mesh->SetRelativeLocation(FVector(-10.000000,10.000000,-2.500000));
	Interact->SetRelativeLocation(FVector(10.000000,-10.000000,2.500000));
	Interact->SetBoxExtent(FVector(10.000000,10.000000,2.500000));
}
