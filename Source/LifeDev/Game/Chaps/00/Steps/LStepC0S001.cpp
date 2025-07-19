// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.05;
	UseFBAnim = true; // the cam target and pos are set on the outliner
	CamBlendTime = 0; // doesn't get along with fade
	InputEnabled = false;
	UseFade = true; // important since the door, also due to the load
	FinishPostWait = false;
	TeleportChar = true;
	RemoveItems = {
		LDConsts::Items::Tape0, "Bottle00", "Bottle01"
	}; // ensure the player doesn't keep them.

	UseRain = true;
	UseGhosts = true;
	GhostPos = FVector(210,-42,-65);

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room00.Room00")));
	
	ActorsShow = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.B_NPC00_C_UAID_D8BBC116E501E38701_1654185673"))
	};
	ActorsHide = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.B_NPC00_C_UAID_D8BBC116E501E38701_1654185673"))
	};
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetMax(1);
}

// don't change the fb val for the next chapter here. since it's not the responsibility of this class.
// in other words: don't put your nose where it doesn't belong. don't do what you're not asked.
// because you might not know what is needed (it was actually introducing a soft error by using the default time.)
// also it makes it harder to find the origin.
