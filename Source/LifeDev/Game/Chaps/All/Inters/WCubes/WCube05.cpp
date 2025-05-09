// Copyright Jerónimo Barraco-Mármol

#include "WCube05.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube05::AWCube05():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube05.WordCube05"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube05", "State0", "Word cube: 'U'"),
		NSLOCTEXT("WCube05", "State1", "Word cube: 'V'"),
		NSLOCTEXT("WCube05", "State2", "Word cube: 'W'"),
		NSLOCTEXT("WCube05", "State3", "Word cube: 'X'"),
		NSLOCTEXT("WCube05", "State4", "Word cube: 'Y'"),
		NSLOCTEXT("WCube05", "State5", "Word cube: 'Z'"),
	};
}
