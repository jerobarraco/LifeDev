// Copyright Jerónimo Barraco-Mármol

#include "WCube07.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube07::AWCube07():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube07.WordCube07"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		NSLOCTEXT("WCube07", "State0", "Word cube: '4'"),
		NSLOCTEXT("WCube07", "State1", "Word cube: '5'"),
		NSLOCTEXT("WCube07", "State2", "Word cube: '6'"),
		NSLOCTEXT("WCube07", "State3", "Word cube: '7'"),
		NSLOCTEXT("WCube07", "State4", "Word cube: '8'"),
		NSLOCTEXT("WCube07", "State5", "Word cube: '9'"),
	};
}
