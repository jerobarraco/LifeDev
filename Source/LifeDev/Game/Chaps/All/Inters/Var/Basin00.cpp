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
		CMesh (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/HandBasin.HandBasin"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(40,30,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,0).Quaternion());

	Interact->SetRelativeLocation(FVector(30,-20,12.5));
	Interact->SetBoxExtent(FVector(30,20,12.5));
	Interact->SetEnabled(false); // by default don't do anything.
	
	Water = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Water"));
	Water->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CWater(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/WaterTap.WaterTap"));
	Water->SetAsset(CWater.Object);
	Water->SetAutoActivate(false);
	Water->SetUseAutoManageAttachment(true);
	Water->SetRelativeLocation(FVector(17,10,18));

	SND_Water = CreateDefaultSubobject<UCSounder>("SND_Water");
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CWaterSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/0008_Water_small_drainpipe_close_to_opening.0008_Water_small_drainpipe_close_to_opening"));
	SND_Water->SetSound(CWaterSnd.Object);
	SND_Water->SetAutoActivate(false);
	SND_Water->bAutoManageAttachment = true;

	Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	// Sig->CompsActivate.AddUnique(Water); // don't do this. it will happily crash every time
	Sig->IsOffWhenOffscreen = true;

	Super::SetMobility(EComponentMobility::Static);// static since we won't animate it
}

void ABasin00::BeginPlay() {
	Super::BeginPlay();
	Sig->CompsVis.AddUnique(GetRootComponent());
	Sig->CompsVis.AddUnique(Water);
}

void ABasin00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Sig->Deactivate();
	Super::EndPlay(EndPlayReason);
}

