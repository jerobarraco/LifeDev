// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "LearnMan.generated.h"

// for new users popups, and stuff.
UCLASS(Blueprintable, BlueprintType, Config=LifeDev, DefaultConfig)
class INVENTORY_API ALearnMan: public AInfo {
	GENERATED_BODY()
public:
	ALearnMan();

	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* const Data);
	UFUNCTION(BlueprintCallable)
	void DeInit();


	UFUNCTION(BlueprintCallable)
	bool Show(const FName& Id);

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;
};
