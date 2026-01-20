// Copyright Jerónimo Barraco-Mármol

#include "LSwitch.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

ALSwitch::ALSwitch():Super() {
	UseFade = false;
	UseRewardDestroy = false;
	UseStateLoop = true;
	UseSFXAttached = true;
	UseAutoActivate = true;
	StateNum = 2;

	// interacts start "off/closed"
	// using "toggle" since sometimes it desyncs from the lights
	Texts = {
		NSLOCTEXT("Switch00", "TurnOn", "Toggle"),
		NSLOCTEXT("Switch00", "TurnOff", "Toggle"),
	};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->bUseAsOccluder = false; // optim

	Interact->SetRelativeLocation(FVector(7.500000,-7.500000,12.500000));
	Interact->SetBoxExtent(FVector(5));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Inters/Generic/Button_Press-007.Button_Press-007"));
	SFXs = {SOpen.Object, SOpen.Object}; // reusing the same. close, open
}
