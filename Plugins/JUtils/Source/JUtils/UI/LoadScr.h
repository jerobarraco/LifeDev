// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "LoadScr.generated.h"

UCLASS(Blueprintable)
class JUTILS_API ULoadScr: public UGameInstanceSubsystem {
	GENERATED_BODY()
public:

	static ULoadScr* Instance(const UObject* const O);
	UFUNCTION(BlueprintCallable)
	void SetWidget(UObject* const O);
	
	UFUNCTION(BlueprintCallable)
	void Show();
	UFUNCTION(BlueprintCallable)
	void Hide();
};
