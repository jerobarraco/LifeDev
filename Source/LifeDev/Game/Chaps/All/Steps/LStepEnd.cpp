// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"

#include "Story/StoryMan.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStoryMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepEnd::ALStepEnd():Super() {
	Label = FName("End");
	// implemented like this so it compiles both texts.
	static const FText ST =
		ULSysSettings::IsDemo() ?
		NSLOCTEXT("StepEnd", "Title", "~ To be continued ... ~"):
		NSLOCTEXT("StepEnd", "Title", "~ The End & The Beginning ~");

	Title = ST;
	InputEnabled = false;
	// this is the time to change a level, counted from Start()
	// added to allow the music to fade.
	// quite arbitrary. might cause issues if i change the fade time.
	WaitTime = 2;
	// FinishPostWait = false; // avoid crash
	UseFade = true;
	// don't change the camera
	UsePawnCam = true;
	CamTarget = nullptr;

	// this could be a good idea, or a terrible idea, or a meh idea. I'm almost confident it's an idea.
	// Unload everything here. The fade screen is showing, so it won't be noticeable.
	// In the hopes that it might make the transition to the next level a bit snappier,
	// since it has less stuff to unload. Is that true?
	// The efficient thing would be to just discard all the objects and memory without unloading anything.
	// But it won't be the "academic" way. And UE, being an engine, used by many, probably needs to follow the safe route.
	// I'm quite confident it does process the destructor of all classes (so it's the safe, slow, route).
	// I unload everything: 1) to be sure. 2) the demo will finish earlier. 3) it's the safe, slow, route.
	
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap00_DL.Chap00_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap01_DL.Chap01_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap02_DL.Chap02_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap03_DL.Chap03_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Chaps/Chap04_DL.Chap04_DL")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room00.Room00")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room01.Room01")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room02.Room02")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Room03.Room03")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Living00.Living00")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall00.Hall00")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Hall01.Hall01")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Rooms/Kitchen.Kitchen")));

	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_A.Outside_A")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_B.Outside_B")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_C.Outside_C")));
	DL_Unload.Add(TSoftObjectPtr<UDataLayerAsset>(
		FSoftObjectPath("/Game/LifeDev/Game/Sys/DataLayers/Outside/Outside_D.Outside_D")));
	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (LIKELY(SysSettings)) DL_Unload.Add(SysSettings->BaseDL);
}

void ALStepEnd::OpenLevel() const {
	const UWorld* const World = GetWorld();
	UGameplayStatics::OpenLevel(World, FName(*NextLevel), true);
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
