// Copyright Jerónimo Barraco-Mármol

#include "BasinI00.h"

#include "CQuickMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/CInteract.h"

#include "LifeDev/Game/Sys/Consts/ConstFlags.h"

ABasinI00::ABasinI00():Super() {
	AnimEnabled = false;
	Locked = false;
	
	Texts = { FText::FromString(TEXT("Close")), FText::FromString(TEXT("Done")) };
	StateNum = 2;
	TriggerDlg = "Basin00_T";
	TriggerFlashInc = .1;
	FlagReward = LDConsts::Flags::CH0::Basin;
	DisableWhileAnim = false; // we will disable manually

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

	// TODO add csounder for the water running.
	
	// static since we won't animate it
	Super::SetMobility(EComponentMobility::Static);
}

void ABasinI00::Trigger_Implementation() {
	// TODO disable water running
	Water->Deactivate();
	Super::Trigger_Implementation();
	SetEnabled(false); // trigger only once 
}
