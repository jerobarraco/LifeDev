// Copyright Jerónimo Barraco-Mármol

#include "BasinI00.h"

#include "CQuickMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/CInteract.h"
#include "JSig/CSignificance.h"

#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "Sounds/CSounder.h"

ABasinI00::ABasinI00():Super() {
	Texts = { FText::FromString(TEXT("Close")), FText::FromString(TEXT("Done")) };
	UseAnim = false;
	Locked = false;
	StateNum = 2;
	TriggerDlg = "Basin00_T";
	RewardFlash = .1;
	RewardFlag = LDConsts::Flags::CH0::Basin;
	DisableWhileAnim = false; // we will disable manually
	UseRewardFade = false; // one shot but won't disappear nor it will fade
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/HandBasin.HandBasin"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(40,30,0));
	Mesh->SetRelativeRotation(FRotator(0,-90,0).Quaternion());
	Interact->SetRelativeLocation(FVector(30,-20,12.5));
	Interact->SetBoxExtent(FVector(30,20,12.5));
	Interact->SetEnabled(true);
	
	Water = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Water"));
	Water->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CWater(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/WaterTap.WaterTap"));
	Water->SetAsset(CWater.Object);
	Water->SetAutoActivate(true);
	Water->SetUseAutoManageAttachment(true);
	Water->SetRelativeLocation(FVector(17,10,18));

	SND_Water = CreateDefaultSubobject<UCSounder>("SND_Water");
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CWaterSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/HandBasin/0008_Water_small_drainpipe_close_to_opening.0008_Water_small_drainpipe_close_to_opening"));
	SND_Water->SetSound(CWaterSnd.Object);
	SND_Water->SetAutoActivate(true);
	SND_Water->bAutoManageAttachment = true;

	Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	// Sig->CompsActivate.AddUnique(Water); // don't do this. it will happily crash every time
	Sig->IsOffWhenOffscreen = true;
	
	// SFX_Trigger = TODO faucet closing
	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
}

void ABasinI00::BeginPlay() {
	Super::BeginPlay();
	Sig->OnChanged.AddUniqueDynamic(this, &ABasinI00::SigChanged);
	Sig->CompsVis.AddUnique(GetRootComponent()); //disables niagara
	// Sig->CompsVis.AddUnique(Mesh); // TODO mesh did not come back with this
}

void ABasinI00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Sig->OnChanged.RemoveAll(this);
	Sig->Deactivate();
	Super::EndPlay(EndPlayReason);
}

void ABasinI00::Trigger_Implementation() {
	Water->Deactivate();
	SND_Water->Fade(false);
	Super::Trigger_Implementation();
	SetEnabled(false); // trigger only once 
}

void ABasinI00::SigChanged(ESigValue Significance) {
	const bool Hidden = Significance == ESigValue::Off;
	// set hidden to avoid affecting whether is active or not.
	Water->SetHiddenInGame(Hidden);
}
