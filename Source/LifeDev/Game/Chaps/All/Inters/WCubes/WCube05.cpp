// Copyright Jerónimo Barraco-Mármol

#include "WCube05.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube05::AWCube05():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube05.WordCube05"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		FText::FromString(TEXT("Word cube: 'U'")),
		FText::FromString(TEXT("Word cube: 'V'")),
		FText::FromString(TEXT("Word cube: 'W'")),
		FText::FromString(TEXT("Word cube: 'X'")),
		FText::FromString(TEXT("Word cube: 'Y'")),
		FText::FromString(TEXT("Word cube: 'Z'")),
	};
}
