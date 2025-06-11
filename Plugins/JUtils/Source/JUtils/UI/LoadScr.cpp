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
	if (IsValid(W))
		Widget = W;
}

void ULoadScr::SetWidget(UUserWidget* const O) {
	Widget = O;
}

void ULoadScr::DoTick(const float dt) {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs tick frame=%lli"), __func__, GFrameCounter);
	const class UWorld* const World = GetWorld();
	if (!World) return;
	// this is a horrible hack to allow the timer to tick.
	// it might as well break other things.
	// the timermanager won't tick if this does not change.
	GFrameCounter+=1;
	World->GetTimerManager().Tick(dt);
}

void ULoadScr::Show() {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs"), __func__);
	if (!IsInGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("ULoadScr::%hs was not on game thread. avoided a crash. "), __func__);
		return;
	}
#if UE_BUILD_DEVELOPMENT || UE_BUILD_DEVELOPMENT
	UE_LOG(LogTemp, Warning, TEXT("ULoadScr::%hs sorry dave, i can't let you do that. There's a bug in ue that will make your game crash.. https://issues.unrealengine.com/issue/UE-254119"), __func__);
	// return;
#endif

	CreateMoviePlayer();

	FLoadingScreenAttributes Attr;
	Attr.bAutoCompleteWhenLoadingCompletes = false;
	Attr.bWaitForManualStop = true;
	Attr.bAllowEngineTick = false; // this baby bad boy will cause the crash in the log above
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
	loop = true;
	AsyncTask(ENamedThreads::Type::AnyBackgroundThreadNormalTask, [this] {
		while (loop) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			AsyncTask(ENamedThreads::GameThread, [this] {
				DoTick(100);
			});
		}
	});
	// this actually creates a new slate thread and displays the "movie" there.
	// and in theory puts the game in a bg thread. then on stop it reverses it.
}

void ULoadScr::Hide() {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs"), __func__);
	// CreateMoviePlayer();
	IGameMoviePlayer* const Player = GetMoviePlayer();
	if (UNLIKELY(!Player)) return;
	loop = false;
	Player->OnMoviePlaybackTick().RemoveAll(this);
	Player->StopMovie();
	Player->ForceCompletion();
}
