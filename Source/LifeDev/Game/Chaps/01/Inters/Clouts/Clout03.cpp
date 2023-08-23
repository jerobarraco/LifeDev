// Copyright Jerónimo Barraco-Mármol

#include "Clout03.h"

#include "Interact/CInteract.h"
#include "JUtils/CQuickMesh.h"

AClout03::AClout03():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Bra00.Bra00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	ItemReward = TEXT("C1C03"); // chapter 1 clout 3
	Mesh->SetRelativeLocation(FVector(67.500000,-37.500000,0.000000));
	Interact->SetRelativeLocation(FVector(-67.500000,37.500000,2.500000));
	Interact->SetBoxExtent(FVector(15.000000,45.000000,3.000000));
	TriggerDlg = FName("C03");
}
