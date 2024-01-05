// Copyright Jerónimo Barraco-Mármol

#include "WCube04.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube04::AWCube04():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube04.WordCube04"));
	Mesh->SetStaticMesh(CMesh.Object);
}
