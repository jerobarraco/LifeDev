// Copyright Jerónimo Barraco-Mármol

#include "WCube06.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube06::AWCube06():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube06.WordCube06"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube06", "State0", "Word cube: 'Y'"),
		NSLOCTEXT("WCube06", "State1", "Word cube: 'Z'"),
		NSLOCTEXT("WCube06", "State2", "Word cube: '0'"),
		NSLOCTEXT("WCube06", "State3", "Word cube: '1'"),
		NSLOCTEXT("WCube06", "State4", "Word cube: '2'"),
		NSLOCTEXT("WCube06", "State5", "Word cube: '3'"),
	};
}
