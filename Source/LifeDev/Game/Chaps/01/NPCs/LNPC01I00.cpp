// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Chaps/All/Env/SRain.h"
#include "LifeDev/Game/Inventory/ItemNames.h"

ALNPC01I00::ALNPC01I00():Super() {}

void ALNPC01I00::TriggerLocked_Implementation() {
	Dialogs->AddId("N01.L");
	Super::TriggerLocked_Implementation();
}

EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
	if (Name == Inventory::ItemNames::Card0) {
		Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagSitDone);
		Dialogs->AddId("N01.0");
		return EItemUseResult::SUCCESS;
	}

	Dialogs->AddId("N01.IB");
	return EItemUseResult::BAD_HANDLED;
}

void ALNPC01I00::DiagSitDone() {
	Dialogs->OnDone.RemoveAll(this);
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0));
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01I00::DiagStandDone);
	Dialogs->AddId("N01.1");
}

void ALNPC01I00::DiagStandDone() {
	Dialogs->OnDone.RemoveAll(this);
	if (IsValid(Card)) {
		Card->SetActorHiddenInGame(false);
	}

	ASRain* const R = Cast<ASRain>(UGameplayStatics::GetActorOfClass(GetWorld(), ASRain::StaticClass()));
	if (R) { R->SetPlaying(false); }

	Destroy();
}