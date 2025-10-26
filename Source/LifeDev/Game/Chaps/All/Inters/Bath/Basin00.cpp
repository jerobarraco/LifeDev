// Copyright Jerónimo Barraco-Mármol

#include "Basin00.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "JSig/CSignificance.h"
#include "Sounds/CSounder.h"

ABasin00::ABasin00():Super() {
	Texts = { NSLOCTEXT("Basin0", "State0", "Close"), NSLOCTEXT("Basin0", "State1", "Open")};
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 2;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/HandBasin/HandBasin.HandBasin"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(40,30,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,0).Quaternion());
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(30,-20,12.5));
	Interact->SetBoxExtent(FVector(30,20,12.5));

	// TODO this could be using the emitter stuff
	const FVector WaterPos(FVector(-12.5,-2.5,5));
	Emitter->SetRelativeLocation(WaterPos);
	SFX->SetRelativeLocation(WaterPos);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CWater(TEXT("/Game/LifeDev/Game/Inters/Bath/HandBasin/WaterTap.WaterTap"));
	Particles = {CWater.Object, nullptr};
	static ConstructorHelpers::FObjectFinder<USoundBase> // mizu no oto
		CWaterSnd(TEXT("/Game/LifeDev/Game/Inters/Bath/HandBasin/0008_Water_small_drainpipe_close_to_opening.0008_Water_small_drainpipe_close_to_opening"));
	SFXs = {CWaterSnd.Object, nullptr};

	// TODO remove the significance. since the niagara effect type uses the significance system and does
	// some of this stuff
	// Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	// Sig->CompsActivate.AddUnique(Water); // don't do this. it will happily crash every time
	// Sig->IsOffIfOffscreen = true;
	// Sig->IsOffIfOccluded = true;
	// Sig->CompsHide.AddUnique(Emitter);

	Super::SetMobility(EComponentMobility::Static);// static since we won't animate it
}

// void ABasin00::BeginPlay() {
	// Super::BeginPlay();
	// don't do this. it will disable the interact and never come back
	// Sig->CompsHide.AddUnique(GetRootComponent());
	// Disable water when not looking at it
	// Sig->CompsHide.AddUnique(Emitter);
// }

// void ABasin00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// Sig->Deactivate();
	// Super::EndPlay(EndPlayReason);
// }

