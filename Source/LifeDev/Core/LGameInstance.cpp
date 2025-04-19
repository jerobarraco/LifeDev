// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LGameInstance.h"

#include "MoviePlayer.h"

#include "JUtils/Misc/JUtilsSys.h"
#include "Settings/LSettings.h"
#include "Settings/LSysSettings.h"
#include "Story/Sentry.h"

ULGameInstance* ULGameInstance::Instance(UObject* const O) {
	if (!IsValid(O)) return nullptr;
	
	const UWorld* const World = O->GetWorld();
	if (!World) return nullptr;

	return Cast<ULGameInstance>(World->GetGameInstance());
}

void ULGameInstance::Init() {
	Super::Init();

	// https://unrealcommunity.wiki/loading-screen-243mzpq1
	// create widget https://forums.unrealengine.com/t/createwidget-c/462559/2
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULGameInstance::EndLoadingScreen);

	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->InstInit();
	
	// force disable debug flags
	// ULSysSettings* const SysSettings = ULSysSettings::Get();
	ULSettings* const Settings = GetSubsystem<ULSettings>();
	if (LIKELY(IsValid(Settings))) Settings->Init();
}

void ULGameInstance::BeginLoadingScreen(const FString& InMapName) {
	if (UNLIKELY(IsRunningDedicatedServer())) return;

	IGameMoviePlayer* const MoviePlayer = GetMoviePlayer();
	if (MoviePlayer) {
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		// use widget->takewidget() to get the swidget
		// https://forums.unrealengine.com/t/how-do-you-convert-a-uuserwidget-to-a-swidget-needed-for-a-loadingscreen/343494/8
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		MoviePlayer->SetupLoadingScreen(LoadingScreen);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("%hs, Can't get movie player"), __func__);
	}

	UJUtilsSys::CameraFade(this, false);
}

void ULGameInstance::EndLoadingScreen(UWorld* InLoadedWorld) {
	UJUtilsSys::CameraFade(this, true);
}

void ULGameInstance::BeginDestroy() {
	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->InstDeInit();

	// attempt to open the feedback url if any. it crashes on module shutdown.
	const ULSysSettings* const SSettings = ULSysSettings::Get();
	if (LIKELY(!CloseTriggered && !IsRunningCookCommandlet() && !UJUtilsSys::IsEditor() && IsValid(SSettings) && !SSettings->CloseURL.IsEmpty()))
		FPlatformProcess::LaunchURL(*SSettings->CloseURL, NULL, NULL);
	CloseTriggered = true;
	Super::BeginDestroy();
}
