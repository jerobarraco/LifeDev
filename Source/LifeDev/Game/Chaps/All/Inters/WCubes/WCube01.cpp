// Copyright Jerónimo Barraco-Mármol

#include "WCube01.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube01::AWCube01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube01.WordCube01"));
	Mesh->SetStaticMesh(CMesh.Object);
}
