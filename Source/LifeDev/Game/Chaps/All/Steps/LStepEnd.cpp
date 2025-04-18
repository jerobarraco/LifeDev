// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"

#include "Story/StoryMan.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStoryMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepEnd::ALStepEnd():Super() {
	Name = FName("End");
	// implemented like this so it compiles both texts.
	static const FText ST =
		ULSysSettings::IsDemo() ?
		NSLOCTEXT("StepEnd", "Title", "~ To be continued ... ~"):
		NSLOCTEXT("StepEnd", "Title", "~ The End & The Beginning ~");

	Title = ST;
	InputEnabled = false;
	UseFadeTime = false; // don't override wait time
	// this is the time to change a level, counted from Start()
	// added to allow the music to fade.
	// quite arbitrary. might cause issues if i change the fade time.
	WaitTime = 2;
	FinishPostWait = false; // avoid crash
	UseFade = true;
	// don't change the camera
	UsePawnCam = true;
	CamTarget = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDLB(TEXT("/Game/LifeDev/Game/Sys/DataLayers/Base"));
	if (LIKELY(CDLB.Succeeded())) DL_Unload.Add(CDLB.Object.Get());
}

void ALStepEnd::OpenLevel() const {
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true);
}

void ALStepEnd::TryStart_Implementation() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	// constexpr float Wait = 2;
	Super::TryStart_Implementation();

	/// turn off everything
	
	/// make sure all the music and sounds fades
	
	FB->SetMin(0); // make sure we can "get low"
	// -.03 to ensure it ends before the open level
	FB->SetVal(0, WaitTime -.03);
	
	ALMusicMan* const MusicMan = ALMusicMan::Instance(World);
	if (LIKELY(MusicMan)) {
		MusicMan->Fade(false);
		MusicMan->SetEnviron(false);
		MusicMan->SetRain(false);
	}

	// TODO need to fix the fade that hides it. (???)
	const ALStoryMan* const StoryMan = ALStoryMan::Instance(World);
	if (LIKELY(StoryMan)) StoryMan->ShowBGSolid(true);
}

void ALStepEnd::Start_Implementation() {
	Super::Start_Implementation();
	OpenLevel();
}
