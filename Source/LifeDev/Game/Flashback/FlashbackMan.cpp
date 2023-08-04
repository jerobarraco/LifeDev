// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "FlashbackMan.h"

#include "Flashback.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

AFlashbackMan::AFlashbackMan():Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection>
		CMPC(TEXT("/Game/LifeDev/Game/Flashback/Flashback_MPC"));
	MPC = CMPC.Succeeded() ? CMPC.Object : nullptr;
}

void AFlashbackMan::ValChanged(float Value) {
	// UE_LOG(LogTemp, Log, TEXT("Flashback value changed to %3.3f"), Value);
	if (IsValid(MPCInst)) {
		MPCInst->SetScalarParameterValue("Val", Value);
	}
}

void AFlashbackMan::BeginPlay() {
	Super::BeginPlay();
	UFlashback* const Flashback = GetWorld()->GetSubsystem<UFlashback>();
	if (IsValid(Flashback)) {
		Flashback->OnChange.AddUniqueDynamic(this, &AFlashbackMan::ValChanged);
	}
	
	if (IsValid(MPC)) {
		MPCInst = GetWorld()->GetParameterCollectionInstance(MPC); 
	}
}

void AFlashbackMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	UFlashback* const Flashback = GetWorld()->GetSubsystem<UFlashback>();
	if (IsValid(Flashback)) {
		Flashback->OnChange.RemoveAll(this);
	}
	MPCInst = nullptr;
	MPC = nullptr;
}
