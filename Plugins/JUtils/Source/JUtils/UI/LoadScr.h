// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "LoadScr.generated.h"

class UUserWidget;

// This is experimental due to an issue with the background tick.
// it might have issues when dealing with the timermanager or depending on the tick
// set UseBgTick in case you need to tick.
UCLASS(Blueprintable, Category=JUtils, Config=JUtils, DefaultConfig)
class JUTILS_API ULoadScr: public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	static ULoadScr* Instance(const UObject* const O);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// sets the widget to be used as loading screen. otherwise it will use a test one.
	UFUNCTION(BlueprintCallable)
	void SetWidget(UUserWidget* const O);
	// shows the loading screen. sends the game thread to the background.
	UFUNCTION(BlueprintCallable)
	void Show();
	// hides the loading screen. brings the game thread to the foreground.
	UFUNCTION(BlueprintCallable)
	void Hide();

	// processes a manual tick. either call this manually, or set  UseBgTick
	// currently it only ticks the timer manager
	UFUNCTION()
	void DoTick(const float dt);

	// when set, it will force call DoTick on the game thread, while on the background.
	UPROPERTY(BlueprintReadWrite, Category=SetUp, Config)
	bool UseBGTick = true;

	virtual bool ShouldCreateSubsystem(UObject* const Outer) const override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> Widget = nullptr;

	// whether this subsystem will be created.
	// when false, it will save some cycles, but might make the app crash if you try to use it without checking the pointer.
	// (checking the pointer is always recommended)
	// should be changed in the config file
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	bool ShouldCreate = false;

	bool UseBGLoop = false;
};

