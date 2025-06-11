// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "LoadScr.generated.h"
// This is experimental due to an issue with the background tick.
// Don't over-rely on it yet

UCLASS(Blueprintable)
class JUTILS_API ULoadScr: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:

	static ULoadScr* Instance(const UObject* const O);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable)
	void SetWidget(UUserWidget* const O);

	UFUNCTION()
	void DoTick(const float dt);
	UFUNCTION(BlueprintCallable)
	void Show();
	UFUNCTION(BlueprintCallable)
	void Hide();

	UPROPERTY(BlueprintReadWrite, Transient)
	bool UseBGTick = true;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> Widget = nullptr;
	bool UseBGLoop = false; 
};

