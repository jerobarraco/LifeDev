// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Chaps/04/Inters/Var/MatchBoxI00.h"
#include "LifeDev/Game/Chaps/04/Inters/Var/PotI00.h"

ALNPCI06::ALNPCI06():Super() {
	Texts = {FText::FromString(FString("Talk"))};

	TriggerDlg = "NPCI06_T";
	RewardStep = false; // just enable the pot
	UseRewardDestroy = false;
	Locked = false;
	IsOneShot = true;
	UseFade = true;
	UseAnim = false;
	
	Super::SetAutoActivate(false); // disabled by default. does not matter. get enabled by c4s2.
}

void ALNPCI06::BeginPlay() {
	Super::BeginPlay();
	RewardIntersActive.AddUnique(
		Cast<AInteract>(
			UGameplayStatics::GetActorOfClass(this, APotI00::StaticClass()))
	);
	RewardIntersActive.AddUnique(
		Cast<AInteract>(
			UGameplayStatics::GetActorOfClass(this, AMatchBoxI00::StaticClass()))
	);
}

// on editor. rewardinterenabled. enables the pot.
