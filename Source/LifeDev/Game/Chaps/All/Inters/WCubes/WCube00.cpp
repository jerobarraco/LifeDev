// Copyright Jerónimo Barraco-Mármol

#include "WCube00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWCube00::AWCube00():Super() {
	UseRewardDestroy = false;
	DisableWhileAnim = false; // To be used with the combination puzzle.
	UseAutoActivate = true; // a bit lazy, but unlikely i'll have word cubes that aren't active
	UseFade = true; // a bit more costly, but most of the time, these cubes fade.
	UseAnim = true;
	UseAnimFBFPS = true;
	AnimFade->MatBase = nullptr; // the cubes use palettes.
	Anim->IsAdditive = false;
	Anim->Duration = 1;
	StateNum = 6;
	Texts = { NSLOCTEXT("WCube00", "State0", "A cube with letters") } ;
	// all the faces. order is important to match the letters' order
	Trans = {
		FTransform::Identity,

		FTransform(FRotator(0,90,0)),
		FTransform(FRotator(0,180,0)),
		FTransform(FRotator(0,270,0)),
		
		FTransform(FRotator(90,0,0)),
		FTransform(FRotator(270,0,0)),
	};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/WCube/WordCube00.WordCube00"));
	Mesh->SetStaticMesh(CMesh.Object);
	// Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetRelativeLocation(FVector(-4,4,-4));
	Mesh->SetCastAllShadows(true);

	SFX->SetRelativeLocation(FVector(4,-4,4));
	// SFX->SetRelativeLocation(FVector(0,0,0));
	Interact->SetRelativeLocation(FVector(4,-4,4));
	// Interact->SetRelativeLocation(FVector(0,0,0));
	Interact->SetBoxExtent(FVector(5,5,5));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	// SFXTrigger = CSnd.Object;
	SFXs = {CSnd.Object, CSnd.Object, CSnd.Object, CSnd.Object, CSnd.Object, CSnd.Object};
}
