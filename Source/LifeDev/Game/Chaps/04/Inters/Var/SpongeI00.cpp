// Copyright Jerónimo Barraco-Mármol

#include "SpongeI00.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Misc/JMiscConsts.h"
#include "LifeDev/Core/Consts/ConstItems.h"

// deactivated by default. activated by a step triggered by the pot.

ASpongeI00::ASpongeI00():Super() {
	RewardItem = LDConsts::Items::Plate01;
	IsLocked = false;
	IsOneShot = true;
	UseRewardDestroy = false;
	UseFade = false;
	UseAnim = true;

	StateNum = 2;
	Trans = {
		FTransform(
			FRotator(-4.923850,0.870385, -10.037423),
			FVector(0,-5,10)
		),
		FTransform::Identity
	};
	IRoot->SetRelativeTransform(Trans[1]);
	Anim->IsAdditive = false;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/washing_hands_inside_a_bathroom_mostly_empty_soap_pump_sink_on_then_off_edit"));
	// TriggerDlg = "Sponge00_T";
	// SFXTrigger = CSnd.Object;
	SFXs = { nullptr, CSnd.Object};
	Plates = {
		SoftOP(ALInteract, "/Script/LifeDev.Plate'/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5019DE401_2031044825'"),
		SoftOP(ALInteract, "/Script/LifeDev.Plate'/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5019DE401_2013816824'"),
		SoftOP(ALInteract, "/Script/LifeDev.Plate'/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5019DE401_2013811823'"),
		SoftOP(ALInteract, "/Script/LifeDev.Plate'/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.StaticMeshActor_UAID_D8BBC116E5010EE301_1945677390'"),
	};
}

void ASpongeI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	for (const TSoftObjectPtr<ALInteract>& pI: Plates) {
		ALInteract* const I = pI.Get();
		if (UNLIKELY(!IsValid(I))) continue;

		I->Fade(false);
	}

	Plates.Empty();
}

void ASpongeI00::BeginPlay() {
	Super::BeginPlay();
	for (const TSoftObjectPtr<ALInteract>& pI: Plates) {
		ALInteract* const I = pI.Get();
		if (UNLIKELY(!IsValid(I))) continue;

		I->Fade(true);
		I->SetActive(false); // force them disabled.
	}
}
