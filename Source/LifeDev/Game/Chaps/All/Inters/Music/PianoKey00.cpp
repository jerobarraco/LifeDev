// Copyright Jerónimo Barraco-Mármol

#include "PianoKey00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

APianoKey00::APianoKey00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Piano_Keys_0.Piano_Keys_0"));
	Mesh->SetStaticMesh(CM.Object);
	Interact->SetRelativeLocation(FVector(0,7.5,-2.5));
	Interact->SetBoxExtent(FVector(10,7.5,2.5));
}
