// Copyright Jerónimo Barraco-Mármol

#include "BasinI00.h"

#include "NiagaraComponent.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Sounds/CLSounder.h"

ABasinI00::ABasinI00():Super() {
	Texts = { FText::FromString(TEXT("Close")), FText::FromString(TEXT("Done")) };
	UseAnim = false;
	Locked = false;
	TriggerDlg = "Basin00_T";
	RewardFlash = .1;
	RewardFlag = LDConsts::Flags::Game::CH0::Basin;
	DisableWhileAnim = false; // we will disable manually
	UseRewardDestroy = false; // one shot but won't disappear nor it will fade

	UseHint = true;
	HintCondition = "{Inter.TriggerL.DoorI05}";
	
	Water->SetAutoActivate(true);
	SND_Water->SetAutoActivate(true);
	IsOneShot = true;
}

void ABasinI00::DoTrigger_Implementation() {
	Water->Deactivate();
	SND_Water->Fade(false);
	Super::DoTrigger_Implementation();
}
