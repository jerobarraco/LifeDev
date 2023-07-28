// Copyright Jerónimo Barraco-Mármol

#include "Window00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

AWindow00::AWindow00():Super() {
	// TODO set up
	SFX->SetRelativeLocation(FVector(37.5,2.5,110));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-SideA.Window00-SideA"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-10.000000,60.000000,0));
	Interact->SetRelativeLocation(FVector(32.5,-55,107.5));
	Interact->SetBoxExtent(FVector(22,5,97.5));
	
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->TEnd.SetRotation(FRotator(0.0,90.0,0.0).Quaternion());


	// TODO set positions
	// TODO set use parent bounds (what was the name? )
	// TODO make a child of ustaticmeshcompont that is more optimized
	GlassA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassA"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-Glass.Window00-Glass"));
	GlassA->SetupAttachment(Mesh);
	GlassA->SetStaticMesh(CGlass.Object);
	GlassA->bUseAttachParentBound = true;

	GlassB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassB"));
	GlassB->SetupAttachment(Mesh);
	GlassB->SetStaticMesh(CGlass.Object);
	GlassB->bUseAttachParentBound = true;
	
	GlassC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassC"));
	GlassC->SetupAttachment(Mesh);
	GlassC->SetStaticMesh(CGlass.Object);
	GlassC->bUseAttachParentBound = true;
}
