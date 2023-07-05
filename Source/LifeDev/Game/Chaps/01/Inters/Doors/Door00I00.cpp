// Copyright Jerónimo Barraco-Mármol

#include "Door00I00.h"

ADoor00I00::ADoor00I00():Super() {
	AnimEnabled = false;
	LockDlg = "D00_LCK";
	Locked = true;
	// todo mesh should be set on the base class
	RootComponent->SetWorldLocation(FVector(-325.000000,-170.000000,0.000005));
	RootComponent->SetWorldRotation(FRotator(0.000000,89.999999,0.000000));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00I00/Door00I00.Door00I00"));
	Mesh->SetStaticMesh(CMesh.Object);
}
