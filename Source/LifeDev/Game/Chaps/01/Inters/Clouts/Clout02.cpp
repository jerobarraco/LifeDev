// Copyright Jerónimo Barraco-Mármol

#include "Clout02.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

AClout02::AClout02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
	ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Boxer00.Boxer00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	AnimEnabled = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	ItemReward = TEXT("C1C02"); // chapter 1 clout 3
	Mesh->SetRelativeLocation(FVector(-15.00000,15.00000,-0.000000));
	Interact->SetRelativeLocation(FVector(15.00000,-15.00000,2.500000));
	Interact->SetBoxExtent(FVector(15.000000,15.000000,3.000000));
	Texts[0] = FText(NSLOCTEXT("Clout02", "PickUp", "Pick up"));
	TriggerDlg = FName("C02");
}
