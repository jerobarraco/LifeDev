// Copyright Jerónimo Barraco-Mármol

#include "Basin00.h"

#include "CQuickMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/CInteract.h"
#include "JSig/CSignificance.h"

#include "Sounds/CSounder.h"

ABasin00::ABasin00():Super() {
	Texts = { FText::FromString(TEXT("Close")), FText::FromString(TEXT("Open"))};
	UseAnim = false;
	UseRewardFade = false;
	StateNum = 2;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/HandBasin/HandBasin.HandBasin"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(40,30,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,0).Quaternion());

	Interact->SetRelativeLocation(FVector(30,-20,12.5));
	Interact->SetBoxExtent(FVector(30,20,12.5));
	Interact->SetEnabled(false); // by default don't do anything.
	
	Water = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Water"));
	Water->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CWater(TEXT("/Game/LifeDev/Game/Inters/HandBasin/WaterTap.WaterTap"));
	Water->SetAsset(CWater.Object);
	Water->SetAutoActivate(false);
	Water->SetUseAutoManageAttachment(true);
	Water->SetRelativeLocation(FVector(17,10,18));

	SFX->SetRelativeLocation(FVector(10,10,0));
	SND_Water = CreateDefaultSubobject<UCSounder>("SND_Water");
	SND_Water->SetupAttachment(Water);
	static ConstructorHelpers::FObjectFinder<USoundBase> // mizu no oto
		CWaterSnd(TEXT("/Game/LifeDev/Game/Inters/HandBasin/0008_Water_small_drainpipe_close_to_opening.0008_Water_small_drainpipe_close_to_opening"));
	SND_Water->SetSound(CWaterSnd.Object);
	SND_Water->SetAutoActivate(false);
	SND_Water->bAutoManageAttachment = true;
	
	
	Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	// Sig->CompsActivate.AddUnique(Water); // don't do this. it will happily crash every time
	Sig->IsOffIfOffscreen = true;
	Sig->IsOffIfOccluded = true;

	Super::SetMobility(EComponentMobility::Static);// static since we won't animate it
}

void ABasin00::BeginPlay() {
	Super::BeginPlay();
	// don't do this. it will disable the interact and never come back
	// Sig->CompsHide.AddUnique(GetRootComponent());
	// Disable water when not looking at it
	Sig->CompsHide.AddUnique(Water);
}

void ABasin00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Sig->Deactivate();
	Super::EndPlay(EndPlayReason);
}

