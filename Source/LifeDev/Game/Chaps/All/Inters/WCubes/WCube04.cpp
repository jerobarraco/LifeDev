// Copyright Jerónimo Barraco-Mármol

#include "WCube04.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube04::AWCube04():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube04.WordCube04"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube04", "State0", "Word cube: 'S'"),
		NSLOCTEXT("WCube04", "State1", "Word cube: 'T'"),
		NSLOCTEXT("WCube04", "State2", "Word cube: 'U'"),
		NSLOCTEXT("WCube04", "State3", "Word cube: 'V'"),
		NSLOCTEXT("WCube04", "State4", "Word cube: 'W'"),
		NSLOCTEXT("WCube04", "State5", "Word cube: 'X'"),
	};
}
