// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LoadScr.h"

#include <thread>

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

// https://www.youtube.com/watch?app=desktop&v=ON1_dEHoNDg

ULoadScr* ULoadScr::Instance(const UObject* const O) {
	if (UNLIKELY(!O)) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!W)) return nullptr;

	const UGameInstance* const Instance = W->GetGameInstance();
	if (UNLIKELY(!Instance)) return nullptr;

	return Instance->GetSubsystem<ULoadScr>();
}

void ULoadScr::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	const TSoftObjectPtr<UUserWidget> SoftWidget = TSoftObjectPtr<UUserWidget>(
		FSoftObjectPath("/JUtils/UI/TestLoadScr.TestLoadScr"));
	UUserWidget* const W = SoftWidget.LoadSynchronous();
	if (LIKELY(IsValid(W)))
		Widget = W;
}

void ULoadScr::SetWidget(UUserWidget* const O) {
	Widget = O;
}

void ULoadScr::DoTick(const float dt) {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs tick frame=%lli"), __func__, GFrameCounter);
	const UWorld* const World = GetWorld();
	if (!World) return;

	// The movie player creates a new transient slate threads, and displays the "movie" there.
	// and it puts the game in a bg thread. then on stop it reverses it.
	
	// this is a horrible hack to allow the timer to tick.
	// it might as well break other things.
	// the timermanager won't tick if this does not change.
	GFrameCounter+=1;
	World->GetTimerManager().Tick(dt);
}

bool ULoadScr::ShouldCreateSubsystem(UObject* const Outer) const {
	// UE_LOG(LogTemp, Log, TEXT("%hs is=%i."),
	// __func__, ShouldBeCreated);
	//
	// if (!ShouldBeCreated) {
	// 	UE_LOG(LogAnim, Log, TEXT("%hs is false. The world subsystem will not be created."
	// 		"Can be changed on the config file Interact.ini"), __func__);
	// 	return false;
	// }
	//
	return Super::ShouldCreateSubsystem(Outer);
}

void ULoadScr::Show() {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs"), __func__);

	if (!IsInGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("ULoadScr::%hs was not on game thread. avoided a crash. "), __func__);
		return;
	}

	CreateMoviePlayer();

	FLoadingScreenAttributes Attr;
	Attr.bAutoCompleteWhenLoadingCompletes = false;
	Attr.bWaitForManualStop = true;
	// There's a bug in ue that will make your game crash ... https://issues.unrealengine.com/issue/UE-254119
	// to be fixed in 5.7
	Attr.bAllowEngineTick = false;
	Attr.MinimumLoadingScreenDisplayTime = 10;

	IGameMoviePlayer* const Player = GetMoviePlayer();
	if (UNLIKELY(!Player)) return;

	if (!Widget)
		Attr.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
	else {
		Attr.WidgetLoadingScreen = Widget->TakeWidget();
	}

	Player->SetupLoadingScreen(Attr);
	Player->PlayMovie();

	Player->OnMoviePlaybackTick().AddUObject(this, &ULoadScr::DoTick); // doesn't work

	if (UNLIKELY(!UseBGTick)) {
		UE_LOG(LogTemp, Log, TEXT("ULoader::%hs UseBGTick is false. Timers won't work. Good luck."),
			__func__);
		// good luck. timers won't work
		return;
	}

	// see DoTick as to why this. yes, it's a hack.
	UseBGLoop = true;
	AsyncTask(ENamedThreads::Type::AnyBackgroundThreadNormalTask, [this] {
		while (UseBGLoop) { // this happens on the bg, otherwise it will lock the game thread or the slate thread
			static constexpr uint8_t SleepTime = 100; // large enough so that the async task is processed.
			// it's loading so we don't really need a small tick, it even could cause problems
			std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
			AsyncTask(ENamedThreads::GameThread, [this] { // tick on the game thread
				DoTick(SleepTime);
			});
		}
	});
}

void ULoadScr::Hide() {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs"), __func__);
	// CreateMoviePlayer();
	IGameMoviePlayer* const Player = GetMoviePlayer();
	if (UNLIKELY(!Player)) return;
	UseBGLoop = false; // would stop the fake timer
	Player->OnMoviePlaybackTick().RemoveAll(this);
	Player->StopMovie();
	Player->ForceCompletion();
}
