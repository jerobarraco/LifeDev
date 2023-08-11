// Copyright Jerónimo Barraco-Mármol

#include "Window00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/CQuickMesh.h"

AWindow00::AWindow00():Super() {
	SFX->SetRelativeLocation(FVector(9,-55,43));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-SideA.Window00-SideA"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));
	Interact->SetRelativeLocation(FVector(9,-35,60));
	Interact->SetBoxExtent(FVector(5,30,55));
	// disable the windows for now since they don't do anything except confusing
	Interact->SetEnabled(false);
	
	GlassA = CreateDefaultSubobject<UCQuickMesh>(TEXT("GlassA"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CGlass(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-Glass.Window00-Glass"));
	GlassA->SetupAttachment(Mesh);
	GlassA->SetStaticMesh(CGlass.Object);
	GlassA->bUseAttachParentBound = true;

	GlassB = CreateDefaultSubobject<UCQuickMesh>(TEXT("GlassB"));
	GlassB->SetupAttachment(Mesh);
	GlassB->SetStaticMesh(CGlass.Object);
	GlassB->SetRelativeLocation(FVector(0,0,-35));
	GlassB->bUseAttachParentBound = true;
	
	GlassC = CreateDefaultSubobject<UCQuickMesh>(TEXT("GlassC"));
	GlassC->SetupAttachment(Mesh);
	GlassC->SetStaticMesh(CGlass.Object);
	GlassC->SetRelativeLocation(FVector(0,0,-70));
	GlassC->bUseAttachParentBound = true;
}
