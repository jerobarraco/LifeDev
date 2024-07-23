// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "FlashbackMan.h"

#include "Flashback.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

AFlashbackMan::AFlashbackMan():Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Succeeded() ? CMPC.Object : nullptr;
}

void AFlashbackMan::ValChanged(float Value) {
	// UE_LOG(LogTemp, Log, TEXT("Flashback value changed to %3.3f"), Value);
	if (!IsValid(MPCInst)) return;
	MPCInst->SetScalarParameterValue("Intensity", Value);
}

void AFlashbackMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;

	UFlashback* const Flashback = World->GetSubsystem<UFlashback>();
	if (IsValid(Flashback))
		Flashback->OnChange.AddUniqueDynamic(this, &AFlashbackMan::ValChanged);
	
	if (!IsValid(MPC)) {
		UE_LOG(LogTemp, Warning, TEXT("FlashbackMan::%hs Could not get the MPC. Stop"),
			__func__);
		return;
	}
	
	MPCInst = World->GetParameterCollectionInstance(MPC);
	
	if (!IsValid(MPCInst)){
		UE_LOG(LogTemp, Warning, TEXT("FlashbackMan::%hs Could not get the MPCInst. Stop."),
			__func__);
		return;
	} 
	
	const int Strobe = ULSettings::GetFeatS(World, EFeat::V_STROBE)?1:0;
	MPCInst->SetScalarParameterValue("Strobe", Strobe);
}

void AFlashbackMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UFlashback* const Flashback = GetWorld()->GetSubsystem<UFlashback>();
	if (IsValid(Flashback))	Flashback->OnChange.RemoveAll(this);

	MPCInst = nullptr;
	MPC = nullptr;
}
