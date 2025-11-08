// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

#include "CQuickMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
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

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CFlame(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Stove/Flame_NS"));

	// flames
	for (uint8 i= 0; i<8; ++i) {
		const FString SName = TEXT("Flame_") + FString::FromInt(i);
		UNiagaraComponent* Comp = CreateDefaultSubobject<UNiagaraComponent>(FName(SName));
		if (UNLIKELY(!Comp)) continue;
		Comp->SetupAttachment(Mesh);
		Comp->SetAsset(CFlame.Object);
		Comp->SetUseAutoManageAttachment(true);
		// Comp->SetAutoActivate(false);
		Comp->SetAutoActivate(true);
		Flames.Add(Comp);
	}
}

void AStoveI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();

	// small trick so that i can play 2 sfxs at once. the default one is short-lived.
	UseSFXAttached = false;
	PlaySFX(SFXTriggerB);
	UseSFXAttached = true;
}
