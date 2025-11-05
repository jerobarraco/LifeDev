// Copyright Jerónimo Barraco-Mármol

#include "Door02.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ADoor02::ADoor02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Doors/Door02/DoorA.DoorA"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-5,15,0));
	Mesh->SetQuickCollisionEnabled(true);

	WinA = CreateDefaultSubobject<UCQuickMesh>(TEXT("WinA"));
	WinA->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Inters/Doors/Door02/DoorA-WinA.DoorA-WinA"));
	WinA->SetStaticMesh(CMesh1.Object);
	WinA->SetRelativeLocation(FVector(0,0,0));
	WinA->SetUseDynShadow(false); // opt
	WinA->bUseAttachParentBound = true; // opt
	
	WinB = CreateDefaultSubobject<UCQuickMesh>(TEXT("WinB"));
	WinB->SetupAttachment(Mesh);
	WinB->SetStaticMesh(CMesh1.Object);
	WinB->SetRelativeLocation(FVector(0,0,-55));
	WinB->SetUseDynShadow(false); // opt
	WinB->bUseAttachParentBound = true; // opt
	
	SFX->SetRelativeLocation(FVector(100,-10,82.5));

	Interact->SetRelativeLocation(FVector(55,-10,100));
	Interact->SetBoxExtent(FVector(50,10,100));

	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
	ADoor02::SetMobility(EComponentMobility::Movable);
}

void ADoor02::SetMobility(const EComponentMobility::Type Mobility) {
	WinA->SetMobility(Mobility);
	WinB->SetMobility(Mobility);
	Super::SetMobility(Mobility);
}
