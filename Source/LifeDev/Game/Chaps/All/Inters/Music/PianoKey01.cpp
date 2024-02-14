// Copyright Jerónimo Barraco-Mármol

#include "PianoKey01.h"

#include "CQuickMesh.h"

APianoKey01::APianoKey01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Piano_Keys_1.Piano_Keys_1"));
	Mesh->SetStaticMesh(CM.Object);
}
