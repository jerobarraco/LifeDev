// Copyright Jerónimo Barraco-Mármol

#include "Clout02.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AClout02::AClout02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Boxer00.Boxer00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	ItemReward = TEXT("C1C02"); // chapter 1 clout 3
	Mesh->SetRelativeLocation(FVector(-15.00000,15.00000,-0.000000));
	Interact->SetRelativeLocation(FVector(15.00000,-15.00000,2.500000));
	Interact->SetBoxExtent(FVector(15.000000,15.000000,3.000000));
	TriggerDlg = FName("C02");
}
