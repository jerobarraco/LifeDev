// Copyright Jerónimo Barraco-Mármol

#include "Door00.h"

ADoor00::ADoor00():Super() {
	AnimEnabled = false;
	LockDlg = "D00_LCK";
	Locked = true;
	RootComponent->SetWorldLocation(FVector(-325.000000,-170.000000,0.000005));
	RootComponent->SetWorldRotation(FRotator(0.000000,89.999999,0.000000));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);
}
