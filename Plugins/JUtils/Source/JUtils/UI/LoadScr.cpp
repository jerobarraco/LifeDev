// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LoadScr.h"

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
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs tick"), __func__);
	class UWorld* const World = GetWorld();
	if (!World ) return;
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
	Player->PlayMovie(); // TODO is this necessary?

	Player->OnMoviePlaybackTick().AddUObject(this, &ULoadScr::DoTick); // doesn't work
	// this actually creates a new slate thread and displays the "movie" there.
	// and in theory puts the game in a bg thread. then on stop it reverses it.
}

void ULoadScr::Hide() {
	UE_LOG(LogTemp, Warning, TEXT("LoadScr::%hs"), __func__);
	// CreateMoviePlayer();
	IGameMoviePlayer* const Player = GetMoviePlayer();
	if (UNLIKELY(!Player)) return;
	Player->OnMoviePlaybackTick().RemoveAll(this);
	Player->StopMovie();
	Player->ForceCompletion();
}
