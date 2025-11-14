// Copyright Jerónimo Barraco-Mármol

#include "Calendar.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ACalendar::ACalendar():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	StateNum = 1;
	Texts = { NSLOCTEXT("Calendar", "State0", "A calendar") } ;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Calendar/Calendar"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-22.5,22.5,0));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(22.500000,-22.500000,2.500000));
	Interact->SetBoxExtent(FVector(22.500000,22.500000,2.500000));
	SFX->SetRelativeLocation(FVector(22.500000,-22.500000,2.500000));
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	// SFX_Trigger = CSnd.Object;
	SFXs = { CSnd.Object };

	Super::SetMobility(EComponentMobility::Static);
}
