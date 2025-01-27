// Copyright Jerónimo Barraco-Mármol

#include "WCube06.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube06::AWCube06():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube06.WordCube06"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		FText::FromString(TEXT("Word cube: 'Y'")),
		FText::FromString(TEXT("Word cube: 'Z'")),
		FText::FromString(TEXT("Word cube: '0'")),
		FText::FromString(TEXT("Word cube: '1'")),
		FText::FromString(TEXT("Word cube: '2'")),
		FText::FromString(TEXT("Word cube: '3'")),
	};
}
