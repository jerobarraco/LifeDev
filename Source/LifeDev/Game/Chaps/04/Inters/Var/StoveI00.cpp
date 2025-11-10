// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

AStoveI00::AStoveI00():Super() {
	IsLocked = true;
	IsOneShot = true;
	UnlockItems = {LDConsts::Items::Matches00};
	RewardFlash = .11;
	UseHint = true;
	// this gets deactivated after trigger. so this expression is fine
	HintCondition = "{Inter.Trigger.LNPCI06}";
	
	// TriggerDlg = "Stove00_T";
	// LockedDlg = "Stove00_L";
	// LockedItemDlg = "Stove00_LI";
	// RewardIntersActiveClass = {APotI00::StaticClass()};
	// UseAutoActivate = false; // activated by LNPCI06 // redundant

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Boiling_Water_in_a_Coffee_Pot"));
	SFXs = { CSnd.Object };

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndB(TEXT("/Game/LifeDev/Game/Inters/Kitchen/MatchBox/match_burn_meridian_-09"));
	SFXTriggerB = CSndB.Object;
	RewardIntersActive = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E501C1E701_2080650083"))};
	RewardIntersTrigger= { // activate flames
		TSoftObjectPtr<ALInteract>(FSoftObjectPath(TEXT("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Flames_UAID_D8BBC116E50149A002_2093575303")))
	};
}

void AStoveI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();

	// small trick so that i can play 2 sfxs at once. the default one is short-lived.
	UseSFXAttached = false;
	PlaySFX(SFXTriggerB);
	UseSFXAttached = true;
}
