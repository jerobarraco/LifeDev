// Copyright Jerónimo Barraco-Mármol

#include "LSwitchI02.h"

#include "NiagaraSystem.h" // include or crash :/  (needed for the object finder)

ALSwitchI02::ALSwitchI02(): Super() {
	IsOneShot = false;
	UseActiveOnce = false;
	UseAutoActivate = true; // important

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSfx(TEXT("/Game/LifeDev/Game/Inters/Generic/Sparks/Tone_Sound_Design_Electric_Spark_Gritty_Thin_Crackle_Dense_Steady_0"));
	SFXs[1] = CSfx.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CPart0(TEXT("/Game/LifeDev/Game/Inters/Generic/Sparks/Sparks_N"));
	Particles = { nullptr, CPart0.Object };

	RewardIntersTrigger = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath(
			"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Light00_B_C_UAID_D8BBC116E5013DAE01_2079906675")),
		TSoftObjectPtr<AInteract>(FSoftObjectPath(
			"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.B_Fluorescent_C_UAID_D8BBC116E501E0B901_1128252023"))
	};
}
