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

void ULoadScr::Show() {
	if (!IsInGameThread()) {
		UE_LOG(LogTemp, Warning, TEXT("ULoadScr::%hs was not on game thread. avoided a crash. "), __func__);
		return;
	}

	CreateMoviePlayer();

	FLoadingScreenAttributes Attr;
	Attr.bAutoCompleteWhenLoadingCompletes = false;
	Attr.bWaitForManualStop = true;
	Attr.bAllowEngineTick = true;
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
	// this actually creates a new slate thread and displays the "movie" there.
	// and in theory puts the game in a bg thread. then on stop it reverses it.
}

void ULoadScr::Hide() {
	CreateMoviePlayer();
	IGameMoviePlayer* const Player = GetMoviePlayer();
	if (UNLIKELY(!Player)) return;
	Player->StopMovie();
}
