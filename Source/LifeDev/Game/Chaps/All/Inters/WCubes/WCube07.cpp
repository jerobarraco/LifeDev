// Copyright Jerónimo Barraco-Mármol

#include "WCube07.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube07::AWCube07():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube07.WordCube07"));
	Mesh->SetStaticMesh(CMesh.Object);
}
