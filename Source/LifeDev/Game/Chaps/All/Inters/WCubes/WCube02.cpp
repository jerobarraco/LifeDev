// Copyright Jerónimo Barraco-Mármol

#include "WCube02.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube02::AWCube02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube02.WordCube02"));
	Mesh->SetStaticMesh(CMesh.Object);
}
