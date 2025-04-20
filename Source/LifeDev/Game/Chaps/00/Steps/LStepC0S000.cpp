// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "Kismet/GameplayStatics.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	
	static FText STitle = NSLOCTEXT("C0S0", "Title", "~ Intention ~");
	Title = STitle;
	
	// DlgId = FName("Intro");
	InputEnabled = true;
	UseFade = true;
	UseFadeTime = true;
	FinishPostWait = false;
	UsePawnCam = true;
	UseRandFB = true;
	Music = FSoftObjectPath(TEXT("/Game/LifeDev/Game/Env/Music/Music04/Music04_MS.Music04_MS"));
	ItemsFinish = { LDConsts::Items::Card0 };
	// this would fix the music not being packaged
	if (UNLIKELY(IsRunningCookCommandlet())) Music.LoadSynchronous();

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap00_DL.Chap00_DL"));
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLOutA(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A"));
	if (LIKELY(CDL1.Succeeded())) DL_Load.Add(CDL1.Object);
	if (LIKELY(CDLOutA.Succeeded())) DL_Load.Add(CDLOutA.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR2(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02"));
	if (LIKELY(CDLR2.Succeeded())) DL_Load.Add(CDLR2.Object);
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLR3(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room03"));
	if (LIKELY(CDLR3.Succeeded())) DL_Load.Add(CDLR3.Object);
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
	Flags->Set(LDConsts::Flags::Game::CH0::Basin);
	Flags->Set(LDConsts::Flags::Game::CH0::Spot);

	AActor* const Pawn = UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass());
	if (LIKELY(Pawn)) Pawn->SetActorLocation(FVector(-683.152746,-20.003613,67.150001));
}
