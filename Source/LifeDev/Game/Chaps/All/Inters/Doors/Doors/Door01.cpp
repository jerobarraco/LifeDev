// Copyright Jerónimo Barraco-Mármol

#include "Door01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ADoor01::ADoor01():Super() {
	IRoot->SetRelativeLocation(FVector(0,-5,0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Doors/Door01/Door01.Door01"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));

	Window = CreateDefaultSubobject<UCQuickMesh>(TEXT("Window"));
	Window->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Inters/Doors/Door01/Door01-Window.Door01-Window"));
	Window->SetStaticMesh(CMesh1.Object);
	Window->SetUseDynShadow(true);
	Window->bUseAttachParentBound = true; // opt
	
	SFX->SetRelativeLocation(FVector(-112.5,0,105));
	Interact->SetRelativeLocation(FVector(-60,-7.5,101));
	Interact->SetBoxExtent(FVector(60,10,101));

	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}
