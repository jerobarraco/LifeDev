// Copyright Jerónimo Barraco-Mármol

#include "Fridge.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Core/Sounds/CLSounder.h"

AFridge::AFridge():Super() {
	Texts = { NSLOCTEXT("Fridge", "State0", "Fridge") } ;
	RewardFlash = 0;
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Fridge/Fridge.Fridge"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(60.5,17,0));

	Interact->SetRelativeLocation(FVector(-90,-12.5,90));
	Interact->SetBoxExtent(FVector(35,7.5,85));
	SFX->SetRelativeLocation(FVector(-90,-15,95));

	SFXHum = CreateDefaultSubobject<UCLSounder>(TEXT("SFXHum"));
	SFXHum->SetupAttachment(Interact);
	SFXHum->SetAutoActivate(true);
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Fridge/Refrigerator_-_Closing_the_Door_03"));
	// SFX_Trigger = CSnd.Object;
	SFXs = { CSnd.Object };

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndHum(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Fridge/kitchen_roomtone_with_refrigerator_002"));
	SFXHum->SetSound(CSndHum.Object);

	// set static by default
	AFridge::SetMobility(EComponentMobility::Static);
	Mesh->SetQuickCollisionEnabled(true); // too big.
	// too big to not cast shadows. will look weird.
	Mesh->SetCastAllShadows(true);
}
