// Copyright Jerónimo Barraco-Mármol

#include "Clout01.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AClout01::AClout01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Shirt01.Shirt01"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	ItemReward = TEXT("C1C01"); // chapter 1 clout 0
	Mesh->SetRelativeLocation(FVector(-17.500000,22.500000,-0.000000));
	Interact->SetRelativeLocation(FVector(17.500000,-22.500000,2.500000));
	Interact->SetBoxExtent(FVector(18.000000,23.000000,3.000000));
	TriggerDlg = FName("C01");
}
