// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "FlashbackMan.h"

#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#include "Flashback.h"

AFlashbackMan::AFlashbackMan():Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Object;
}

void AFlashbackMan::ValChanged(const float Value) {
	// UE_LOG(LogTemp, Log, TEXT("Flashback value changed to %3.3f"), Value);
	if (UNLIKELY(!IsValid(MPCInst))) return;
	MPCInst->SetScalarParameterValue("Intensity", Value);
}

void AFlashbackMan::BeginPlay() {
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	if (UNLIKELY(!IsValid(MPC))) {
		UE_LOG(LogTemp, Warning, TEXT("FlashbackMan::%hs Could not get the MPC. Stop"),
			__func__);
		return;
	}
	
	MPCInst = World->GetParameterCollectionInstance(MPC);
	UE_CLOG(UNLIKELY(!IsValid(MPCInst)), LogTemp, Warning, TEXT("FlashbackMan::%hs Could not get the MPCInst. Stop."),
		__func__);
}

void AFlashbackMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Flashback = UFlashback::Instance(this);
	if (LIKELY(IsValid(Flashback)))	Flashback->OnChange.RemoveAll(this);

	MPCInst = nullptr;
	MPC = nullptr;

	Super::EndPlay(EndPlayReason);
}

void AFlashbackMan::Init() {
	UFlashback* const Flashback = UFlashback::Instance(this);
	if (UNLIKELY(!IsValid(Flashback))) return;

	ValChanged(Flashback->GetVal());
	Flashback->OnChange.AddUniqueDynamic(this, &AFlashbackMan::ValChanged);
}
