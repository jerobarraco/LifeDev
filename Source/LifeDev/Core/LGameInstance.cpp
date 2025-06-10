// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LGameInstance.h"

#include "LoadScr.h"

#include "JUtils/Misc/JUtilsSys.h"

#include "Settings/LSettings.h"
#include "Settings/LSysSettings.h"
#include "Sentry.h"
#include "Blueprint/UserWidget.h"

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
	
	// force disable debug flags
	// ULSysSettings* const SysSettings = ULSysSettings::Get();
	ULSettings* const Settings = GetSubsystem<ULSettings>();
	if (LIKELY(IsValid(Settings))) Settings->Init();

	ULoadScr* const LoadScr = ULoadScr::Instance(this);
	if (UNLIKELY(!LoadScr)) return;
	LoadScr->Show();

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (UNLIKELY(!SysSettings)) return;
	
	UUserWidget* const Widget = SysSettings->LoadingScr.LoadSynchronous();
	if (UNLIKELY(!IsValid(Widget))) return;
	LoadScr->SetWidget(Widget);

	
}

void ULGameInstance::BeginLoadingScreen(const FString& MapName) {
	if (UNLIKELY(IsRunningDedicatedServer())) return;
	return;

	// todo this does not seem to be working.
	// im going to be showing and hiding the load screen per level 
	UJUtilsSys::CameraFade(this, false);
}

void ULGameInstance::EndLoadingScreen(UWorld* const LoadedWorld) {
	// ULoadScr* const LoadScr = ULoadScr::Instance(this);
	// UJUtilsSys::CameraFade(this, true);
}

// https://forums.unrealengine.com/t/event-on-close/298087/5?u=nande
void ULGameInstance::Shutdown() {
	USentry* const Sentry = USentry::Instance(this);
	if (LIKELY(Sentry)) Sentry->InstDeInit();

	// attempt to open the feedback url if any. it crashes on module shutdown.
	const ULSysSettings* const SSettings = ULSysSettings::Get();
	if (LIKELY(!CloseTriggered & !UJUtilsSys::IsEditor() & !IsRunningCookCommandlet() &
		(IsValid(SSettings) && !SSettings->CloseURL.IsEmpty())))
		FPlatformProcess::LaunchURL(*SSettings->CloseURL, NULL, NULL);
	CloseTriggered = true;
	Super::Shutdown();
}
