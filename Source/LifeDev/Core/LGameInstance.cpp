// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LGameInstance.h"

#include "MoviePlayer.h"
#include "JUtils/JMiscUtils.h"
#include "Settings/LSettings.h"

ULGameInstance* ULGameInstance::Instance(UWorld* World) {
	if (!IsValid(World)) return nullptr;
	return Cast<ULGameInstance>(World->GetGameInstance());
}

void ULGameInstance::Init() {
	Super::Init();

	// https://unrealcommunity.wiki/loading-screen-243mzpq1
	// create widget https://forums.unrealengine.com/t/createwidget-c/462559/2
    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULGameInstance::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULGameInstance::EndLoadingScreen);

	// force disable debug flags
	ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (SysSettings && !UJMiscUtils::IsDebug()) {
		SysSettings->UseDebugFeats = false;
		SysSettings->UseSaveGame = true;
	}
	
	ULSettings* const Settings = GetSubsystem<ULSettings>();
	if (IsValid(Settings)) {
		Settings->Init();
	}
}

void ULGameInstance::BeginLoadingScreen(const FString& InMapName) {
	if (IsRunningDedicatedServer()) return;
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Screen loading is on"));	
	

	IGameMoviePlayer* const MoviePlayer = GetMoviePlayer();
	if (MoviePlayer) {
		FLoadingScreenAttributes LoadingScreen;
        LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
        // use widget->takewidget() to get the swidget
        // https://forums.unrealengine.com/t/how-do-you-convert-a-uuserwidget-to-a-swidget-needed-for-a-loadingscreen/343494/8
        LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		MoviePlayer->SetupLoadingScreen(LoadingScreen);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Can't get movie player"));
	}

	// TODO move this to JUtils (?)
	// UWorld* const World = GetWorld();
	// APlayerController* const Controller = World ? GetFirstLocalPlayerController(World) : nullptr;
	APlayerController* const Controller = GetPrimaryPlayerController();
	APlayerCameraManager* CamManager = Controller ? Controller->PlayerCameraManager : nullptr;
	if (CamManager) {
		CamManager->StartCameraFade(0, 1, .5f, FLinearColor::Black, true, true);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Can't get camera manager, not fading"));
	}
}

void ULGameInstance::EndLoadingScreen(UWorld* InLoadedWorld) {
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Screen loading is done"));	

	// UWorld* const World = GetWorld();
	// APlayerController* const Controller = World ? GetFirstLocalPlayerController(World) : nullptr;
	APlayerController* const Controller = GetPrimaryPlayerController();
	APlayerCameraManager* CamManager = Controller ? Controller->PlayerCameraManager : nullptr;
	if (CamManager) {
		CamManager->StartCameraFade(1, 0, .5f, FLinearColor::Black, true, true);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Can't get camera manager, not fading"));
	}
}
