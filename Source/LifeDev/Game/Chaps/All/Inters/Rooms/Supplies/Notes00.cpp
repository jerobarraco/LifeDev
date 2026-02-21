// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Notes00.h"

#include "CQuickMesh.h"
#include "Components/TextRenderComponent.h"
#include "Interact/CInteract.h"

ANotes00::ANotes00() {
	Texts = {
		NSLOCTEXT("Ppl", "Notes00", "A sticky note"), // closed
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseFade = false; // by default skip it
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Rooms/Supplies/Note00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetUseDynShadow(true);
	Mesh->SetRelativeLocation(FVector(-4.,4.,0.));

	Interact->SetRelativeLocation(FVector(4.000000,-4.000000,0.250000));
	Interact->SetBoxExtent(FVector(4.,4.,1.));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC"));
	SFXs = { CSnd.Object };
	
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Mesh);
	Text->SetRelativeLocation(FVector(0.2,-0.200000,.6));
	Text->SetRelativeRotation(FRotator(90,0,0));
	// Text->SetText(NSLOCTEXT("", "", ""));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetComponentTickEnabled(false);
	Text->SetWorldSize(1);
	Text->SetYScale(1);
	Text->SetVertSpacingAdjust(0);
	
	Super::SetMobility(EComponentMobility::Static);
}
