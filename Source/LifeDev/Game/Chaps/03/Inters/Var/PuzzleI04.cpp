// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI04.h"

#include "Interact/CPuzzle.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI04::APuzzleI04():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {1, 2, 0}; 
	CPuzzle->ResetOnFail = true;

	static FName DoneId = "PZ04_T";
	DoneDlg = DoneId; // really? TODO maybe not necessary
	// DoneFB = .15;
	// TODO this will get the engine stuck
	DoneStep = "C3S0"; // TODO trigger boss

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSWrong (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Bad.Group_Bad"));
	SND_Wrong = CSWrong.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSRight (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_Good.Group_Good"));
	SND_Right = CSRight.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSReset (TEXT("/Game/LifeDev/Game/Inters/Key00/Key19.Key19"));
	SND_Reset = CSReset.Object;
}

void APuzzleI04::PostLoad() {
	Super::PostLoad();
	// common item dialogs
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ04xC00"},
		{LDConsts::Items::Card1, "PZ04xC01"},
		{LDConsts::Items::Card2, "PZ04xC02"},
		{LDConsts::Items::Card3, "PZ04xC03"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = {false, false, false, false};
	SetLocks(Locks);
}

// TODO refactor all this

void APuzzleI04::Done_Implementation(bool Ok) {
	// disable until i play the solution
	// TODO not working
	SetEnableds(false);

	// TODO improve
	SND = Ok ? SND_Right : SND_Wrong;
	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &APuzzleI04::PlayDone, 2);
	// TODO disable interacts while waiting
}

void APuzzleI04::DoReset_Implementation() {
	Super::DoReset_Implementation();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SND_Reset, GetActorLocation());
}

void APuzzleI04::PlayDone() {
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SND, GetActorLocation());
	if (SND == SND_Right) return;
	
	SetEnableds(true);
}
