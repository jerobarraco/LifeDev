// Copyright Jerónimo Barraco-Mármol

#include "Clout04.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

// TODO move this to /All/Clouts and make a child class named CloutI0X and replace in the editor

AClout04::AClout04():Super() {
	RewardItem = TEXT("C1C04"); // chapter 1 clout 4
	TriggerDlg = FName("C04");
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Sock00.Sock00"));
	if (ObjMesh.Succeeded())
		Mesh->SetStaticMesh(ObjMesh.Object);
	Mesh->SetRelativeLocation(FVector(-10.000000,10.000000,-2.500000));
	Interact->SetRelativeLocation(FVector(10.000000,-10.000000,2.500000));
	Interact->SetBoxExtent(FVector(10.000000,10.000000,2.500000));
	AnimFade->SetNewMat();
}
