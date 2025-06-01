// Copyright Jerónimo Barraco-Mármol

#include "RadioI00.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Consts/ConstItems.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Sounds/CLSounder.h"

ARadioI00::ARadioI00():Super() {
	UseAutoActivate = true;
	IsLocked = true;
	// when the user activates. though this happens automatically after ULockDlg.
	//  it's all ok though, it's according to keikaku (keikaku means plan).
	// TriggerDlg = "RD00_T";
	// ULockItemReq = LDConsts::Items::Tape0;
	UnlockItems = {LDConsts::Items::Tape0};
	RewardFlash = 0; // the dialog will add 
	UseRewardDestroy = false; // don't self-destroy
	DisableWhileAnim = false; // to allow to disable once activated
	Texts = { NSLOCTEXT("RadioI00", "Play", "Play tape")};

	// use a sounder and not the ghost object since i want to only use the sound.
	SFX_Ghost = CreateDefaultSubobject<UCLSounder>(TEXT("SFX_Ghost"));
	SFX_Ghost->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Env/Ghost/Ghosts_S.Ghosts_S"));
	SFX_Ghost->SetSound(CSnd.Object);
	SFX_Ghost->TimeFadeIn = 1;
	SFX_Ghost->TimeFadeOut = 2.0;
	SFX_Ghost->TimeStartMin = 0;
	SFX_Ghost->TimeStartMax = 120;

	SetStateNow(1); // start open
}

void ARadioI00::DoTrigger_Implementation() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	Inventory->Rem(LDConsts::Items::Tape0); // consume

	Diags->OnShow.AddUniqueDynamic(this, &ARadioI00::DialogShown);
	Diags->OnDone.AddUniqueDynamic(this, &ARadioI00::DialogDone);
	// fallback in case the dialog fails. max seconds.
	World->GetTimerManager().SetTimer(DiagDoneHandle, this, &ARadioI00::DialogDone, 120);

	Super::DoTrigger_Implementation(); // will trigger the dialog
	SetActive(false); // disable once activated. this item is one use only.
	SFX_Ghost->Fade(true);
}

void ARadioI00::DialogShown(const FDiag& Diag) {
	Flashback->ModVal(.07);
}

void ARadioI00::DialogDone() {
	Diags->OnDone.RemoveAll(this);
	Diags->OnShow.RemoveAll(this);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	World->GetTimerManager().ClearTimer(DiagDoneHandle);
	DiagDoneHandle.Invalidate();
	// this is safe to call once stopped (i think).
	// that's important since we also set the timer.
	SFX_Ghost->Fade(false);
}
