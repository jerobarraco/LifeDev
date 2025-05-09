// Copyright Jerónimo Barraco-Mármol

#include "WCube03.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube03::AWCube03():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube03.WordCube03"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube03", "State0", "Word cube: 'M'"),
		NSLOCTEXT("WCube03", "State1", "Word cube: 'N'"),
		NSLOCTEXT("WCube03", "State2", "Word cube: 'O'"),
		NSLOCTEXT("WCube03", "State3", "Word cube: 'P'"),
		NSLOCTEXT("WCube03", "State4", "Word cube: 'Q'"),
		NSLOCTEXT("WCube03", "State5", "Word cube: 'R'"),
	};
}
