// Copyright Jerónimo Barraco-Mármol

#include "LPuzzle.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/CPuzzle.h"
#include "Interact/Interact.h"
#include "LifeDev/Core/Consts/ConstDlgs.h"

#include "LifeDev/Game/Interact/LInteract.h"

ALPuzzle::ALPuzzle():Super() {
	IsLocked = true; // locked means unsolved
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	DisableWhileAnim = false; // ignored if !UseAnim. better safe than sorry
	IsOneShot = true;
	// UseAutoActivate = false; // by default i'm not using this puzzle as directly. // redundant

	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	CPuzzle->DisableOnDone = true;
	// rarely used by default. used on the hints.
	if (LIKELY(Interact)) Interact->SetBoxExtent(FVector(5));

	ALPuzzle::SetMobility(EComponentMobility::Type::Static);
}

void ALPuzzle::BeginPlay() {
	Super::BeginPlay();

	if (LIKELY(IsValid(CPuzzle))) {
		CPuzzle->OnDone.AddUniqueDynamic(this, &ALPuzzle::Done);
		CPuzzle->OnUpdate.AddUniqueDynamic(this, &ALPuzzle::Update);
		CPuzzle->OnReset.AddUniqueDynamic(this, &ALPuzzle::DoReset);
	} else {
		UE_LOG(LogTemp, Warning,
			TEXT("CPuzzle in Puzzle is not valid!! Huge problem. o=%s"),
			*GetNameSafe(this));
	}
}

void ALPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ClearTimer(); // uses cpuzzle. do first.

	if (LIKELY(CPuzzle)) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
		CPuzzle->OnReset.RemoveAll(this);
	}
	CPuzzle = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ALPuzzle::SetActorHiddenInGame(const bool NewHidden) {
	Super::SetActorHiddenInGame(NewHidden);
	// the LPuzzle actor itself will follow UseAutoActive which is false by default.
	if (UNLIKELY(!CPuzzle)) return;

	CPuzzle->SetHiddensInGame(NewHidden);
}

void ALPuzzle::Reset() {
	Super::Reset();
	ClearTimer();

	if (LIKELY(CPuzzle))
		CPuzzle->Reset(); // triggers a delegate

	if (LIKELY(Diags)) {
		const FName N(LDConsts::Dlgs::Inter::Puzzle::ResetPre + Label.ToString());
		Diags->AddId(N);
	}
}

void ALPuzzle::SetActive_Implementation(const bool Active) {
	// works because of the constructor "setHinted"
	Super::SetActive_Implementation(Active);

	// filter when has not activated due to UseActiveOnce, probably. let deactivate pass.
	if (UNLIKELY(Active & !GetActive())) return;

	SetActives(Active);
}

void ALPuzzle::SetUseHint_Implementation(const bool NewUseHint) {
	SetUseHints(NewUseHint);
}

void ALPuzzle::Fade_Implementation(const bool FadeIn, const bool SetHidden) {
	Super::Fade_Implementation(FadeIn, SetHidden);
	
	// the LPuzzle actor itself will follow UseAutoActive which is false by default. (??? why did i put this here?)
	if (UNLIKELY(!CPuzzle)) return;

	// fade is a LD stuff, can't do at the CPuzzle level
	const TArray<TSoftObjectPtr<AInteract>>& Pieces = CPuzzle->GetPieces();
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		ALInteract* const IL = Cast<ALInteract>(I.Get());
		if (UNLIKELY(!IsValid(IL))) continue;
		IL->Fade(FadeIn, SetHidden);
	}
}

void ALPuzzle::ClearTimer() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	W->GetTimerManager().ClearTimer(ResetTimer);
	ResetTimer.Invalidate();
}

void ALPuzzle::DoReset_Implementation() {}

void ALPuzzle::Update_Implementation() {
	// note update is called before done. so it's safe to re add the timer. done will clear it if needed.
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	ClearTimer();
	// re-add the reset timer if needed. Notice all the types return when done
	if (ResetTimeout >= 0)
		W->GetTimerManager().SetTimer(ResetTimer, this, &ALPuzzle::Reset, ResetTimeout);

	if (LIKELY(Diags)) {
		TArray<FString> SCur;
		for (const int32& S: CPuzzle->GetCurrents()) {
			SCur.Add(FString::FromInt(S));
		}
		const FString& SCurAll = FString::Join(SCur, TEXT("_"));
		// in the hope it will copy fewer strings
		const FString& Base = FString::Printf(TEXT("%s%s.%s"),
			*LDConsts::Dlgs::Inter::Puzzle::UpdatePre, *Label.ToString(), *SCurAll);
		const FName N(Base);
		Diags->AddId(N);
	}
}

void ALPuzzle::Done_Implementation(const bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"),
		IsOk, *Label.ToString());

	if (IsOk) {
		// don't reset if we can't trigger again. avoids issue where the inters get reactivated.
		// only on isOneShot to support puzzles that can be triggered multiple times.
		if (IsOneShot) ClearTimer();

		Unlock(); // force unlock. so that i can trigger.
	} else {
		ClearTimer(); // now that it failed, no more resets. also avoid double timers if ResetOnFail.

		// reset if needed. but not right now. Since done is overrideable and children can change order.
		// it will mess with the logical flow anyway.
		// this is important to be done on the Puzzle since Done is overrideable and hence can be postponed if needed
		if (ResetOnFail) {
			const UWorld* const W = GetWorld();
			if (LIKELY(W)) W->GetTimerManager().SetTimerForNextTick(this, &ALPuzzle::Reset);
		}
		IsLocked = true; // force so it calls triggerLocked
	}

	TryTrigger();
}
