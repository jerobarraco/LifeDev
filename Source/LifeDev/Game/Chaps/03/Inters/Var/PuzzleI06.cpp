// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI06.h"

#include "Interact/CPuzzle.h"
#include "Interact/InteractAnim.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Core/Sounds/CLSounder.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Core/Consts/ConstItems.h"

constexpr float SndWait = 1.75;

APuzzleI06::APuzzleI06():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {1, 2, 0}; /// piano sequence
	CPuzzle->DisableOnInter = true; // will make it easier. non-repeated keys. and make the waiting explicit.
	ResetOnFail = true; // Allow for reset. this is handled with a careful setup of Super::Done

	DoneStep = "C3S0";

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSWrong (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Bad.Group_Bad"));
	SND_Wrong = CSWrong.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSRight (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Good.Group_Good"));
	SND_Right = CSRight.Object;
}

void APuzzleI06::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ04xC00"},
		{LDConsts::Items::Card1, "PZ04xC01"},
		{LDConsts::Items::Card2, "PZ04xC02"},
		// {LDConsts::Items::Card3, "PZ04xC03"} // is rewarded after this
	};
	SetUseItemDlgs(Dlgs);
	
	static const TArray<bool> Locks = {false, false, false, false};
	SetLocks(Locks);
}

void APuzzleI06::Done_Implementation(bool Ok) {
	// notice not calling super::done here since that can reset

	// disable until i play the solution
	SetEnableds(false);

	WasOk = Ok;

	// give time for audio to play
	const UWorld* const W = GetWorld();
	if (!W) return;

	ALGGameMode* const Mode = ALGGameMode::Instance(W);
	if (Mode) Mode->SetCharInputEnabled(false);
	
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI06::PostDone, SndWait);
}

void APuzzleI06::PostDone() {
	const UWorld* const W = GetWorld();
	if (!W) return;

	SND->SetSound(WasOk ? SND_Right : SND_Wrong);
	SND->Play();

	// give time for audio to play
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI06::PostDoneSnd, SndWait);
}

void APuzzleI06::PostDoneSnd() {
	if (!WasOk || !IsValid(Lid)) {
		// retry or skip animation
		LidDone();
		return;
	}

	Lid->Locked = false;
	Lid->TryTrigger();
	
	// i could subscribe to the anim onEnd but this is safer.
	FTimerHandle H;
	const UWorld* const W = GetWorld();
	if (!W) return;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI06::LidDone, Lid->Anim->Duration);
}

void APuzzleI06::LidDone() {
	// before calling done since that could trigger a new step or sequence
	// actually the new step will disable the input, but better to do here in case
	// i change that
	ALGGameMode* const Mode = ALGGameMode::Instance(GetWorld());
	if (Mode) Mode->SetCharInputEnabled(true);
	
	// finally mark the puzzle as done for good. if !WasOk it will retry
	Super::Done_Implementation(WasOk);
}
