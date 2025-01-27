// Copyright Jerónimo Barraco-Mármol

#include "WCube07.h"

#include "JUtils/Actors/CQuickMesh.h"

AWCube07::AWCube07():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube07.WordCube07"));
	Mesh->SetStaticMesh(CMesh.Object);
	Texts = {
		FText::FromString(TEXT("Word cube: '4'")),
		FText::FromString(TEXT("Word cube: '5'")),
		FText::FromString(TEXT("Word cube: '6'")),
		FText::FromString(TEXT("Word cube: '7'")),
		FText::FromString(TEXT("Word cube: '8'")),
		FText::FromString(TEXT("Word cube: '9'")),
	};
}
