// Copyright Jerónimo Barraco-Mármol

#include "PoemI01.h"

APoemI01::APoemI01():Super() {
	RewardItem = "P01";
	// TriggerDlg = "P01_Look"; // using the autodialog
	RewardFlash = -.1f;
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.B_Cassette00_C_UAID_D8BBC116E501478C01_1236035498"))
	};
}
