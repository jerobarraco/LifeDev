// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI04.h"

#include "Interact/CPuzzle.h"
#include "Interact/InteractAnim.h"
#include "Interact/Animator/CAnimatorMix.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

constexpr float SndWait = 1.75;

APuzzleI04::APuzzleI04():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {1, 2, 0}; /// piano sequence
	// Keys have "OneShot"
	// will make it easier. non-repeated keys. and make the waiting explicit.
	// Allow for reset. this is handled with a careful setup of Super::Done
	ResetOnFail = true;
	UseHint = true;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSWrong (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Bad.Group_Bad"));
	SND_Wrong = CSWrong.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSRight (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Good.Group_Good"));
	SND_Right = CSRight.Object;
	// the other puzzle
	RewardActor = TSoftObjectPtr<AInteract>(FSoftObjectPath(
		"/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI03_UAID_D8BBC116E501EDD901_1630772030"));
}

void APuzzleI04::PostLoad() {
	Super::PostLoad();

	static const TArray<bool> Locks = {false, false, false, false};
	SetLocks(Locks);
	SetUseHints(true);
}

void APuzzleI04::Done_Implementation(const bool Ok) {
	// notice not calling super::done here since that can reset

	// disable until i play the solution
	SetActives(false);

	WasOk = Ok;

	// give time for audio to play
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	ALGGameMode* const Mode = ALGGameMode::Instance(W);
	if (LIKELY(Mode)) Mode->SetCharInputEnabled(false);
	
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::PostDone, SndWait);
}

void APuzzleI04::PostDone() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	PlaySFX(WasOk ? SND_Right : SND_Wrong);

	// give time for audio to play
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::PostDoneSnd, SndWait);
}

void APuzzleI04::PostDoneSnd() {
	if (UNLIKELY(!WasOk || !IsValid(Lid))) {
		UE_CLOG(!Lid, LogTemp, Warning, TEXT("APuzzleI04::%hs Lid is invalid. it can't be completed."), __func__);
		// retry or skip animation
		LidDone();
		return;
	}

	Lid->IsLocked = false;
	Lid->TryTrigger();
	
	// i could subscribe to the anim onEnd but this is safer.
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	const UCAnimatorMix* const LidAnim = Lid->GetAnim();
	const float Duration = LidAnim ? LidAnim->Duration : 1;
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &APuzzleI04::LidDone, Duration);
}

void APuzzleI04::LidDone() {
	UE_LOG(LogTemp, Log, TEXT("%hs"), __func__);
	// before calling Done since that could trigger a new step or sequence
	// actually the new step will disable the input,
	// but better to do here, in case it changes, and to avoid stepping on it.
	ALGGameMode* const Mode = ALGGameMode::Instance(GetWorld());
	if (LIKELY(Mode)) Mode->SetCharInputEnabled(true);


	// finally mark the puzzle as done for good. if !WasOk it will retry
	Super::Done_Implementation(WasOk);
}
// Lid is set on the editor. and dialogs set on the datatable

/* rubbish bin
* 
// show a dialog if the user got it wrong.
// do it here to avoid issues with the above SetCharInputEnabled.
// and not on PostDoneSnd to leave the code clean.
if (!WasOk) {
const int32 Num = FailDiags.Num();
if (Diags && Num > 0) {
// this will still start from the correct dialog because i've adjusted the order.
// to avoid having to check for the length, and to avoid having to have a signed integer.
FailDiagIndex = (FailDiagIndex +1) % Num;
Diags->AddId(FailDiags[FailDiagIndex]); // i could do this with a condition now
}

uint8 FailDiagIndex = 0;
	TArray<FName> FailDiags = {
		// notice this is the last one. so that the code can be a bit lazier but still correct.
		"PZ04xC02",
		"PZ04xC00",
		"PZ04xC01",
	};
	// reusing the dialogs from the card is fine atm.
	// the whole reason of this feature is JUST IN CASE the player
	// didn't figure out that it needed to use the cards
	// (because it's an obscure mechanic that is not used elsewhere atm).
}*/

// tried it and did not work (on postLoad) might be too early.
// TArray<UClass*> Classes = {
// 	APianoKeyI00::StaticClass(),
// 	APianoKeyI01::StaticClass(),
// 	APianoKeyI02::StaticClass(),
// 	APianoKeyI03::StaticClass()
// };
//
// TArray<AInteract*> Inters;
// for (UClass* const Class : Classes) {
// 	Inters.AddUnique(
// 		Cast<AInteract>(
// 			UGameplayStatics::GetActorOfClass(GetWorld(), Class)));
// }
// CPuzzle->SetInteracts(Inters);
