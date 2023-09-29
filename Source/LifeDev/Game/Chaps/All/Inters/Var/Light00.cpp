// Copyright Jerónimo Barraco-Mármol

#include "Light00.h"

#include "Components/RectLightComponent.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight00::ALight00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Fluorescent/Fluorescent.Fluorescent"));
	Tube = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Tube->SetupAttachment(Mesh);
	if (CTube.Succeeded()) {
		Tube->SetStaticMesh(CTube.Object);
	}
	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	RectLight->SetupAttachment(Mesh);
	// Texts = {FText::FromString("PickUp")};
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Light00s/Light00s.Light00s"));
	// SFX_Trigger = CSnd.Object;
	// TriggerFlashInc = .1;
	ALight00::SetMobility(EComponentMobility::Static);
}

void ALight00::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Tube->SetMobility(Mobility);
	RectLight->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}
