// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LGameInstance.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "Sentry.h"

#include "JUtils/Misc/JUtilsSys.h"

#include "Settings/LSettings.h"
#include "Settings/LSysSettings.h"

ULGameInstance* ULGameInstance::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!World)) return nullptr;

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
	
	// initialize the input user settings. do here otherwise the input won't work. i think. todo also do it on the intro.
	// TODO this doesn't work either
	// doesn't seem to be needed
	// UEnhancedInputLocalPlayerSubsystem* InputSub = UJUtilsSys::GetEInputSub(this);
	// if (LIKELY(InputSub)) InputSub->InitalizeUserSettings();

	// TODO maybe this should be in the game instance
	
	const UEnhancedInputUserSettings* const EISettings = UJUtilsSys::GetEInputSettings(this);
	if (EISettings) EISettings->LoadOrCreateSettings(GetWorld()->GetFirstLocalPlayerFromController());
	
	// force disable debug flags
	// ULSysSettings* const SysSettings = ULSysSettings::Get();
	ULSettings* const Settings = GetSubsystem<ULSettings>();
	if (LIKELY(IsValid(Settings))) Settings->Init();
}

void ULGameInstance::BeginLoadingScreen(const FString& MapName) {
	UE_LOG(LogTemp, Log, TEXT("%hs map=%s"), __func__, *MapName)
	if (UNLIKELY(IsRunningDedicatedServer())) return;
	
	// todo this does not seem to be working.
	// it can't find the camera manager.
	// im going to be showing and hiding the load screen per level 
	UJUtilsSys::CameraFade(this, false);
}

void ULGameInstance::EndLoadingScreen(UWorld* const LoadedWorld) {
	UE_LOG(LogTemp, Log, TEXT("%hs world=%p"), __func__, LoadedWorld)
	UJUtilsSys::CameraFade(this, true); // if this doesn't work use LoadedWorld
}

// https://forums.unrealengine.com/t/event-on-close/298087/5?u=nande
void ULGameInstance::Shutdown() {
	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->InstDeInit();

	// attempt to open the feedback url if any. it crashes on module shutdown.
	const ULSysSettings* const SSettings = ULSysSettings::Get();
	if (LIKELY(!CloseTriggered & !UJUtilsSys::IsDebug() & !UJUtilsSys::IsEditor()
		& !IsRunningCookCommandlet() & (IsValid(SSettings) && !SSettings->CloseURL.IsEmpty())))
		FPlatformProcess::LaunchURL(*SSettings->CloseURL, NULL, NULL);
	CloseTriggered = true;
	Super::Shutdown();
}
