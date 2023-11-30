// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"

#include "Significance.generated.h"

class USignificanceManager;

// Base subsystem for the significance stuff
UCLASS(Blueprintable, Category="LifeDev")
class JSIG_API USignificance : public UTickableWorldSubsystem {
	GENERATED_BODY()

public:
	USignificance();

	// will re-set some of the objects and cache values
	UFUNCTION(BlueprintCallable)
	void Reset();
	
	static USignificance* Get(UWorld* W);

	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	// TODO move to JUtils blueprint library
	static UGameViewportClient* GetAnyGameViewportClient();

	// seconds until next tick. 0 means every frame. discouraged.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float TickInterval = 1.f;
	
	// 0 or <0 means all
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 NumPCs = 1;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	USignificanceManager* Man = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<APlayerController*> PCs;

	float DTAcum = 0;
};
