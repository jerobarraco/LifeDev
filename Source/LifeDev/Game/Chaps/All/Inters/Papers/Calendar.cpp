// Copyright Jerónimo Barraco-Mármol

#include "Calendar.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ACalendar::ACalendar():Super() {
	UseAnim = false;
	UseRewardFade = false;
	StateNum = 1;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Calendar/Calendar.Calendar"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-22.5,22.5,0));
	
	Interact->SetRelativeLocation(FVector(22.500000,-22.500000,2.500000));
	Interact->SetBoxExtent(FVector(22.500000,22.500000,2.500000));
	
	Texts = { FText::FromString(TEXT("A calendar")) } ;
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Calendar/Paper_SC.Paper_SC"));
	SFX_Trigger = CSnd.Object;

	SetEnabled(false);
	Super::SetMobility(EComponentMobility::Static);
}
