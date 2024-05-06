// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI04.h"

#include "Interact/CPuzzle.h"
#include "Interact/InteractAnim.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

constexpr float SndWait = 1.75;

APuzzleI04::APuzzleI04():Super() {
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

void APuzzleI04::PostLoad() {
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

void APuzzleI04::Done_Implementation(bool Ok) {
	// notice not calling super::done here since that can reset

	// disable until i play the solution
	SetEnableds(false);

	WasOk = Ok;

	// give time for audio to play
	UWorld* const W = GetWorld();
	if (!W) return;

	ALGGameMode* const Mode = ALGGameMode::Instance(W);
	if (Mode) Mode->SetCharInputEnabled(false);
	
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::PostDone, SndWait);
}

void APuzzleI04::PostDone() {
	const UWorld* const W = GetWorld();
	if (!W) return;

	UGameplayStatics::PlaySoundAtLocation(W, WasOk ? SND_Right : SND_Wrong, GetActorLocation());
	// give time for audio to play
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::PostDoneSnd, SndWait);
}

void APuzzleI04::PostDoneSnd() {
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
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::LidDone, Lid->Anim->Duration);
}

void APuzzleI04::LidDone() {
	// before calling done since that could trigger a new step or sequence
	// actually the new step will disable the input, but better to do here in case
	// i change that
	ALGGameMode* const Mode = ALGGameMode::Instance(GetWorld());
	if (Mode) Mode->SetCharInputEnabled(true);
	
	// finally mark the puzzle as done for good. if !WasOk it will retry
	Super::Done_Implementation(WasOk);
}
