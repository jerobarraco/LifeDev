// Copyright Jerónimo Barraco-Mármol

#include "Clout00.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

AClout00::AClout00():Super() {
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Shirt00.Shirt00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}
	AnimEnabled = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	ItemReward = TEXT("C1C00"); // chapter 1 clout 0
	Mesh->SetRelativeLocation(FVector(-17.500000,17.500000,-0.000000));
	Interact->SetRelativeLocation(FVector(17.500000,-17.500000,2.500000));
	Interact->SetBoxExtent(FVector(18.000000,18.000000,3.000000));
	Texts[0] = FText(NSLOCTEXT("Clout00", "PickUp", "Pick up"));
	TriggerDlg = FName("C00");
}
