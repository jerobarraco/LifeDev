// Copyright Jerónimo Barraco-Mármol

#include "BasinI00.h"

#include "CQuickMesh.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/CInteract.h"
#include "JSig/CSignificance.h"

#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "Sounds/CSounder.h"

ABasinI00::ABasinI00():Super() {
	Texts = { FText::FromString(TEXT("Close")), FText::FromString(TEXT("Done")) };
	UseAnim = false;
	Locked = false;
	TriggerDlg = "Basin00_T";
	RewardFlash = .1;
	RewardFlag = LDConsts::Flags::CH0::Basin;
	DisableWhileAnim = false; // we will disable manually
	UseRewardFade = false; // one shot but won't disappear nor it will fade
	
	Water->SetAutoActivate(true);
	SND_Water->SetAutoActivate(true);
}

void ABasinI00::Trigger_Implementation() {
	Water->Deactivate();
	SND_Water->Fade(false);
	Super::Trigger_Implementation();
	SetEnabled(false); // trigger only once 
}
