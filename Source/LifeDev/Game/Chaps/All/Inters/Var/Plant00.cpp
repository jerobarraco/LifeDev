// Copyright Jerónimo Barraco-Mármol

#include "Plant00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Sounds/CSounder.h"

APlant00::APlant00():Super() {
	Texts = { NSLOCTEXT("Plant00", "State0", "Plant")};
	UseAnim = false;
	DisableWhileAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Plants/Bluezone_BC0254_wood_crushing_branches_001_001.Bluezone_BC0254_wood_crushing_branches_001_001"));
	// SFXTrigger = CSFX.Object;
	SFXs = { CSFX.Object };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Plants/Pot00.Pot00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-32.5,32.5,0));
	Mesh->SetQuickCollisionEnabled(true);
	Mesh->SetCastAllShadows(true);

	SFX->SetRelativeLocation(FVector(32.5,-32.5,30));
	// interact extent is a bit buffed to be easily triggerable
	Interact->SetRelativeLocation(FVector(32.5,-32.5,50));
	Interact->SetBoxExtent(FVector(37.5,37.5,50));
	// Interact->SetRelativeLocation(FVector(32.5,-32.5,30));
	// Interact->SetBoxExtent(FVector(37.5,37.5,30));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CSoil(TEXT("/Game/LifeDev/Game/Inters/Plants/Soil.Soil"));
	Soil = CreateDefaultSubobject<UCQuickMesh>(TEXT("Soil"));
	Soil->SetupAttachment(Mesh);
	Soil->SetStaticMesh(CSoil.Object);
	Soil->SetRelativeLocation(FVector(5,-5,50));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CPlant(TEXT("/Game/LifeDev/Game/Inters/Plants/Plant00.Plant00"));
	Plant = CreateDefaultSubobject<UCQuickMesh>(TEXT("Plant"));
	Plant->SetupAttachment(Mesh);
	Plant->SetStaticMesh(CPlant.Object);
	Plant->SetRelativeLocation(FVector(5,-5,50));
	Plant->SetCastAllShadows(true);

	Super::SetMobility(EComponentMobility::Static); // static since we won't animate it
}

void APlant00::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Soil->SetMobility(Mobility);
	Plant->SetMobility(Mobility);
}
