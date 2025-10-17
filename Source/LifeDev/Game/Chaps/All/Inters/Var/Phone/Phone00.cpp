// Copyright Jerónimo Barraco-Mármol

#include "Phone00.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

APhone00::APhone00():Super() {
	StateNum = 1;
	Texts = { NSLOCTEXT("Phone00", "State0", "Phone") };
	UseAnim = false;
	UseFade = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Rooms/Phone/Phone00_P"));
	Mesh->SetStaticMesh(CM.Object);
	Interact->SetRelativeLocation(FVector(15.,-15,5));
	Interact->SetBoxExtent(FVector(15, 15, 5));
	SFX->SetRelativeLocation(FVector(0,0, 5));

	SetMobility(EComponentMobility::Static);
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Rooms/Phone/HangUp"));
	SFXs = {CSnd.Object, CSnd.Object};
	// /Script/Engine.SoundCue'/Game/LifeDev/Game/Inters/Rooms/Phone/RingOnce.RingOnce'
	// /Script/Engine.SoundCue'/Game/LifeDev/Game/Inters/Rooms/Phone/Ring.Ring'
}
