// Copyright Jerónimo Barraco-Mármol

#include "Spot02.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot02::ASpot02():Super() {
	Texts = { FText(NSLOCTEXT("Chap02", "Spot02.Sit", "Sit here")) };
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Arch/Chairs/MOROCCO_BONUS_Wood_Door_6"));
	SFXs = {CSnd.Object};
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Arch/Chairs/Chair00"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	SFX->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetBoxExtent(FVector(17.5, 17.5, 40));
}