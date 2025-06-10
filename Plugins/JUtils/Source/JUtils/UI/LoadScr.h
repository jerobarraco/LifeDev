// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "LoadScr.generated.h"

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

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> Widget = nullptr;
};

