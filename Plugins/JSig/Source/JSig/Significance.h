// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"

#include "Significance.generated.h"

class USignificanceManager;

// Base subsystem for the significance stuff
// you can set the defaults for this if you create a file in Config/DefaultJSignificance
// this file is mandatory for Android or it won't work.
// with the section [/Script/JSig.Significance]
// and then the variables like
// UseBgThread=true
// NumPCs=1
// TickInterval=3.0
UCLASS(Blueprintable, Category="JSig", Config=JSignificance, DefaultConfig)
class JSIG_API USignificance : public UTickableWorldSubsystem {
	GENERATED_BODY()

public:
	USignificance();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static USignificance* Instance(UObject* O);

	// seconds until next tick. 0 means every frame. discouraged.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float TickInterval = 1.5f;
	// <=0 means all
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	int32 NumPCs = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	bool UseBGThread = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	bool TickWhenPaused = false;

protected:
#pragma region base
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
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

	UPROPERTY(BlueprintReadOnly, Transient)
	USignificanceManager* Man = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<APlayerController*> PCs;

	float DTAcum = 0;
};
