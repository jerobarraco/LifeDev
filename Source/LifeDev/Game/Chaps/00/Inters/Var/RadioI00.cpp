// Copyright Jerónimo Barraco-Mármol

#include "RadioI00.h"

#include "Diags/Diags.h"
#include "Sounds/CSounder.h"

ARadioI00::ARadioI00():Super() {
	// when user tries to play but has no tape
	LockedDlg = "RD00_L";
	// when user tries to play but has tape
	LockedItemDlg = "RD00_LI";
	// when user unlocks using the tape
	ULockDlg = "RD00_UL";
	// when the user activates. though this happens automatically after ULockDlg.
	//  it's all ok though, it's according to keikaku (keikaku means plan).
	TriggerDlg = "RD00_T";
	ULockItem = "T01"; // Tape
	RewardFlash = .1;
	UseRewardFade = false; // don't self-destroy
	DisableWhileAnim = false; // to allow to disable once activated
	Texts = {
		FText::FromString(TEXT("Play"))
	};

	SFX_Ghost = CreateDefaultSubobject<UCSounder>(TEXT("SFX_Ghost"));
	SFX_Ghost->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghosts_S.Ghosts_S"));
	SFX_Ghost->SetSound(CSnd.Object);
	SFX_Ghost->TimeFadeIn = 1;
	SFX_Ghost->TimeFadeOut = 2.0;
	SFX_Ghost->TimeStart = 0;
	SFX_Ghost->TimeEnd = 120;
}

void ARadioI00::Trigger_Implementation() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	// set up sound disabling
	Dialogs->OnDone.AddUniqueDynamic(this, &ARadioI00::DialogDone);
	// fallback in case the dialog fails. max seconds.
	World->GetTimerManager().SetTimer(DiagDoneHandle, this, &ARadioI00::DialogDone, 120);
	SFX_Ghost->Fade(true);
	
	Super::Trigger_Implementation();
	SetEnabled(false);// disable once activated. this item is one use only.
}

void ARadioI00::DialogDone() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	World->GetTimerManager().ClearTimer(DiagDoneHandle);
	DiagDoneHandle.Invalidate();
	Dialogs->OnDone.RemoveAll(this);
	// this is safe to call once stopped (i think).
	// that's important since we also set the timer.
	SFX_Ghost->Fade(false);
}
