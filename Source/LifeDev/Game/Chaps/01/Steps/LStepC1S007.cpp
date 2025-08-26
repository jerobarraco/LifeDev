// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S007.h"

#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC1S007::ALStepC1S007():Super() {
	Label = FName("C1S7");
	InputEnabled = true;
	// uses pawn camera
	UsePawnCam = true;
	UseRain = false; // turn off rain
	// wait for the player to talk with the npc and get the card
	FinishItems = {LDConsts::Items::Card1, LDConsts::Items::Poem1};

	// the clothes are transient items that don't need to be carried
	// the rest of the items are going to be used or required
	RemoveItems = {"C1C00", "C1C01", "C1C02", "C1C03", "C1C04", LDConsts::Items::Bra};

	ActorsShow = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Card01_UAID_D8BBC116E501139001_1298044567"))
	};
}

void ALStepC1S007::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetVal(.2);
}

// fades the npc on outliner