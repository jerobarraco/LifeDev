// Copyright Jerónimo Barraco-Mármol

#include "WCube03.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube03::AWCube03():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube03.WordCube03"));
	Mesh->SetStaticMesh(CMesh.Object);
}
