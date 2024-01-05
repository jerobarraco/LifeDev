// Copyright Jerónimo Barraco-Mármol

#include "WCube06.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube06::AWCube06():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube06.WordCube06"));
	Mesh->SetStaticMesh(CMesh.Object);
}
