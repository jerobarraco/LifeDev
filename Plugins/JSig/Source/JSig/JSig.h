// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "JSig.generated.h"

class USignificanceManager;

// Base subsystem for the significance.
// This system allows you to optimize actors by calculating their importance (significance) on the scene,
// and reacting accordingly.
// This is done with certain frequency, and efficiently.
// To use, just enable this plugin and a CSignificance component to your actors.
// Read the docs on the component.
//
// you can set the defaults for this class if you create a file in Config/DefaultJSignificance
// this file is mandatory for Android, or it won't work!
// with the section [/Script/JSig.Significance]
// and then the variables like
// UseBgThread=true
// NumPCs=1
// TickInterval=3.0
UCLASS(Blueprintable, Category="JSig", Config=JSig, DefaultConfig)
class JSIG_API UJSig : public UTickableWorldSubsystem {
	GENERATED_BODY()

public:
	UJSig();

	static UJSig* Instance(const UObject* O);

	// seconds until next tick. 0 means every frame. discouraged.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float TickInterval = 1.5f;
	// Number of player controllers to use for calculating significance.
	// The more, the slower (if they exist).
	// But probably if you have multiple players with multiple cameras, you'd want to increase this.
	// <=0 means all
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	int32 NumPCs = 1;
	// Whether to use a background thread to perform calculations, when possible.
	// It's pretty stable, but if you have crashes, try toggling this off.
	// This will make some of the delegates and functions o the components to run on a background thread.
	// That means that there are certain things you should not do there, or you need to set a timer/async code.
	// It's possibly this flag would make the calculation run on a next frame (untested).
	// If so, there's the possibility that when the tick interval is too frequent,
	// that multiple calculations would run on the same frame (not tragic but a bit wasteful, and a word of warning). 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	bool UseBGThread = true;
	// Whether to keep calculating when the game is paused.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	bool TickWhenPaused = false;

protected:
#pragma region base
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(const float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// the default is conditional. and works fine.
	// virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; };
	// UTickableWorldSubsystem recommends to check for IsInitialized, but it doesn't implement this correctly.
	virtual bool IsTickable() const override { return IsInitialized(); };
	virtual bool IsTickableWhenPaused() const override { return IsInitialized() && TickWhenPaused; };
#pragma endregion

	void DoTick();

	// will re-set some of the objects and cache values
	void Reset();

	UPROPERTY(BlueprintReadOnly, Transient, Category="JSig")
	TObjectPtr<USignificanceManager> Man = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient, Category="JSig")
	TArray<TObjectPtr<APlayerController>> PCs;

	float DTAcum = 0;
};
