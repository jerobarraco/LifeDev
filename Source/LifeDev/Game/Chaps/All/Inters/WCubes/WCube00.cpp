// Copyright Jerónimo Barraco-Mármol

#include "WCube00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWCube00::AWCube00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube00.WordCube00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	// Mesh->SetRelativeLocation(FVector(-4,4,-4));
	
	// SFX->SetRelativeLocation(FVector(4,-4,4));
	SFX->SetRelativeLocation(FVector(0,0,0));
	// Interact->SetRelativeLocation(FVector(4,-4,4));
	Interact->SetRelativeLocation(FVector(0,0,0));
	Interact->SetBoxExtent(FVector(5,5,5));
	
	Texts = { FText::FromString(TEXT("A word Cube00")) } ;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;

	UseAnim = true;
	DisableWhileAnim = false; // To be used with the combination puzzle.
	StateNum = 6;
	Anim->IsAdditive = false;
	// all the faces
	Trans = {
		FTransform::Identity,

		FTransform(FRotator(0,90,0)),
		FTransform(FRotator(0,180,0)),
		FTransform(FRotator(0,270,0)),
		
		FTransform(FRotator(90,0,0)),
		FTransform(FRotator(270,0,0)),
	};
}
