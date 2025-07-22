// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "LoadScr.generated.h"
// This is experimental due to an issue with the background tick.
// Don't over-rely on it yet

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

	UFUNCTION()
	void DoTick(const float dt);

	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseBGTick = true;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> Widget = nullptr;

	// whether this subsystem will be created.
	// when false, it will save some cycles, but might make the app crash if you try to use it without checking the pointer.
	// (checking the pointer is always recommended)
	// should be changed in the config file
	UPROPERTY(BlueprintReadWrite, Config, Category=SetUp)
	bool ShouldCreate = true;

	bool UseBGLoop = false;
};

