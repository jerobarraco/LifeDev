// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStoryMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/StoryManager.h"

ALStepEnd::ALStepEnd():Super() {
	Name = FName("End");
	static FText ST = FText::FromString("~ To be continued ... ~");
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
}

void ALStepEnd::OpenLevel() {
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true);
}

void ALStepEnd::Start_Implementation() {
	UWorld* const World = GetWorld();
	if (!World) return;
	// constexpr float Wait = 2;
	Super::Start_Implementation();

	/// turn off everything
	
	/// make sure all the music and sounds fades
	
	FB->SetMin(0); // make sure we can "get low"
	// -.03 to ensure it ends before the open level
	FB->SetVal(0, WaitTime -.03);
	
	ALMusicMan* const MusicMan = ALMusicMan::Instance(World);
	if (MusicMan) {
		MusicMan->Fade(false);
		MusicMan->SetEnviron(false);
		MusicMan->SetRain(false);
	}

	// TODO need to fix the fade that hides it.
	ALStoryMan* const StoryMan = ALStoryMan::Instance(World);
	if (StoryMan) StoryMan->ShowBGSolid(true);
}

void ALStepEnd::DoStart_Implementation() {
	Super::DoStart_Implementation();
	OpenLevel();
}
