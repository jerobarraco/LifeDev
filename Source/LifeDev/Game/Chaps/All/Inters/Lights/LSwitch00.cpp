// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch00.h"

#include "CQuickMesh.h"

ALSwitch00::ALSwitch00(): Super() {
	// interacts start "off/closed"
	Texts = {
		NSLOCTEXT("Switch00", "TurnOn", "Turn On"),
		NSLOCTEXT("Switch00", "TurnOff", "Turn Off"),
	};

	Base = CreateDefaultSubobject<UCQuickMesh>(TEXT("Base"));
	Base->SetupAttachment(Root);
	// TODO this is a hack. find something better.
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/Btn2"));
	Base->SetStaticMesh(CMesh.Object);
	Base->SetRelativeScale3D(FVector(5.000000,1.000000,7.000000));
	Base->SetRelativeLocation(FVector(-37.500000,27.500000,-542.500000));
	// SFX_Trigger = nullptr // TODO
}
