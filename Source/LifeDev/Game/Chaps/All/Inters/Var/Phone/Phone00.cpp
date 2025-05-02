// Copyright Jerónimo Barraco-Mármol

#include "Phone00.h"

#include "CQuickMesh.h"

APhone00::APhone00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Var/Phone00_P"));
	Mesh->SetStaticMesh(CM.Object);
	SetMobility(EComponentMobility::Static);
}
