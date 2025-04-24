// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "LearnMan.generated.h"

class UFlags;
struct FLearnRow;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLearnShow, const FName&, Id, const FLearnRow&, Row);

// for new users popups, and stuff.
UCLASS(Blueprintable, BlueprintType, Config=Inventory, DefaultConfig)
class INVENTORY_API ALearnMan: public AInfo {
	GENERATED_BODY()
public:
#pragma region base
	ALearnMan();

	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* const Data);
	UFUNCTION(BlueprintCallable)
	void DeInit();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	bool Show(const FName& Id); // not const in case of delegates
	UFUNCTION(BlueprintCallable)
	void Hide();

	UPROPERTY(BlueprintReadWrite, Config)
	float Time = 30;

#pragma region delegates
	UPROPERTY(BlueprintReadWrite, Transient)
	FLearnShow OnShow;
#pragma endregion

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName CurrentId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};
