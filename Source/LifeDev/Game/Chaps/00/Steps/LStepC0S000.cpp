// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "Sound/SoundBase.h"

#include "Inventory/Inventory.h"
#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC0S000::ALStepC0S000():Super() {
	Label = FName("C0S0");
	
	static FText STitle = NSLOCTEXT("C0S0", "Title", "~ Intention ~");
	Title = STitle;
	
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFade = true;
	UsePawnCam = true;
	UseFBRand = true;
	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music02/Music02_MS.Music02_MS"));
	// wait for card AND basin. the spot is linked to the card and puzzle.
	FinishItems = { LDConsts::Items::Card0 };
	FinishFlags = {
		"Inter.Trigger.BasinI00" // TODO move to consts?
		// Inter.Trigger.PuzzleI00 // no need. it's linked to the card.
		// Inter.Trigger.SpotI02 // no need. it's linked to the card.
	};
	// this would fix the music not being packaged

	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap00_DL.Chap00_DL")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02.Room02")));
	DL_Load.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room03.Room03")));
}

void ALStepC0S000::TryStart_Implementation() {
	Super::TryStart_Implementation();

	if (UNLIKELY(!FB)) {
		UE_LOG(LogTemp, Warning, TEXT("ALStepC0S000 Start: can't get the FB subsystem."));
		return;
	}

	FB->SetMax(.75);
	FB->SetMin(0);
	FB->SetVal(.1, .15);
}

void ALStepC0S000::DoDebug_Implementation() {
	Super::DoDebug_Implementation();
	Inventory->Ensure(LDConsts::Items::Poem0);
	Inventory->Ensure(LDConsts::Items::Card0);

	AActor* const Pawn = UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass());
	if (LIKELY(Pawn)) Pawn->SetActorLocation(FVector(-683.152746,-20.003613,67.150001));
}
