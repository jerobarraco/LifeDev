// Copyright Jerónimo Barraco-Mármol

#include "Tape00.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ATape00::ATape00():Super() {
	UseAnim = false;
	StateNum = 1;
	Texts = { NSLOCTEXT("Tape00", "State0", "Tape") };
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Tape00/Cassette00_Tape.Cassette00_Tape"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-5.725000,0,0));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(5.725000,-2.500000,1.250000));
	Interact->SetBoxExtent(FVector(7.000000,6.000000,2.500000));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Walkman00/Tape_SC.Tape_SC"));
	// SFXTrigger = CSTrigger.Object;
	SFXs = { CSnd.Object };
	SFX->SetRelativeLocation(FVector(0,-3.,1));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCase(TEXT("/Game/LifeDev/Game/Inters/Tape00/Cassette00_Case.Cassette00_Case"));
	Case = CreateDefaultSubobject<UCQuickMesh>(TEXT("Case"));
	Case->SetupAttachment(IRoot);
	Case->SetStaticMesh(CCase.Object);
	Case->SetCastAllShadows(true);
	Case->SetRelativeLocation(FVector(-5.725000,0,0));
	AnimFade->Meshes.Add(Case);

	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
}

void ATape00::SetMobility(const EComponentMobility::Type Mobility) {
	Case->SetMobility(Mobility);
	Super::SetMobility(Mobility);
}
