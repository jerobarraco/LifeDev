// Copyright Jerónimo Barraco-Mármol

#include "WCube02.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube02::AWCube02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube02.WordCube02"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube02", "State0", "Word cube: 'G'"),
		NSLOCTEXT("WCube02", "State1", "Word cube: 'H'"),
		NSLOCTEXT("WCube02", "State2", "Word cube: 'I'"),
		NSLOCTEXT("WCube02", "State3", "Word cube: 'J'"),
		NSLOCTEXT("WCube02", "State4", "Word cube: 'K'"),
		NSLOCTEXT("WCube02", "State5", "Word cube: 'L'"),
	};
}
