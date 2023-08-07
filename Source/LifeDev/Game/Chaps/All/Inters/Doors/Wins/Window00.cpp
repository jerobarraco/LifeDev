// Copyright Jerónimo Barraco-Mármol

#include "Window00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

AWindow00::AWindow00():Super() {
	SFX->SetRelativeLocation(FVector(9,-55,43));
	// TODO fix glasses and frame not matching positions
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-SideB.Window00-SideB"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));
	Interact->SetRelativeLocation(FVector(9,-35,60));
	Interact->SetBoxExtent(FVector(5,30,55));

	// TODO make a child of ustaticmeshcompont that is more optimized and use that
	GlassA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassA"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-Glass.Window00-Glass"));
	GlassA->SetupAttachment(Mesh);
	GlassA->SetStaticMesh(CGlass.Object);
	GlassA->bUseAttachParentBound = true;

	GlassB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassB"));
	GlassB->SetupAttachment(Mesh);
	GlassB->SetStaticMesh(CGlass.Object);
	GlassB->SetRelativeLocation(FVector(0,0,-35));
	GlassB->bUseAttachParentBound = true;
	
	GlassC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassC"));
	GlassC->SetupAttachment(Mesh);
	GlassC->SetStaticMesh(CGlass.Object);
	GlassC->SetRelativeLocation(FVector(0,0,-70));
	GlassC->bUseAttachParentBound = true;
}
