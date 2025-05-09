// Copyright Jerónimo Barraco-Mármol

#include "WCube01.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube01::AWCube01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube01.WordCube01"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube01", "State0", "Word cube: 'A'"),
		NSLOCTEXT("WCube01", "State1", "Word cube: 'B'"),
		NSLOCTEXT("WCube01", "State2", "Word cube: 'C'"),
		NSLOCTEXT("WCube01", "State3", "Word cube: 'D'"),
		NSLOCTEXT("WCube01", "State4", "Word cube: 'E'"),
		NSLOCTEXT("WCube01", "State5", "Word cube: 'F'"),
	};
}
