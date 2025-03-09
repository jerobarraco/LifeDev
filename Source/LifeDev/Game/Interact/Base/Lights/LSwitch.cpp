// Copyright Jerónimo Barraco-Mármol

#include "LSwitch.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch::ALSwitch():Super() {
	UseFade = false;
	UseRewardDestroy = false;
	UseStateLoop = true;
	UseAttachedSFX = true;
	StateNum = 2;

	// interacts start "off/closed"
	Texts = {
		NSLOCTEXT("Switch00", "TurnOn", "Turn On"),
		NSLOCTEXT("Switch00", "TurnOff", "Turn Off"),
	};
	
	UseAnim = true;
	Anim->Duration = .5;
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(-2.5,0,0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));

	Interact->SetRelativeLocation(FVector(7.500000,-7.500000,12.500000));
	Interact->SetBoxExtent(FVector(5));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Inters/Generic/Button_Press-007.Button_Press-007"));
	SFX_Start = {SOpen.Object, SOpen.Object}; // reusing the same. close, open
}

void ALSwitch::BeginPlay() {
	Super::BeginPlay();
	SetState(1); // start on. most lights start on. can't do on constructor.
}
