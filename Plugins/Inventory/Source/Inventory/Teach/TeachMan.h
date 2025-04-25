// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "TeachMan.generated.h"

class UFlags;
struct FTeachRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLearnShow, const FName&, Id, const FTeachRow&, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLearnHide);

// for new users popups, and stuff.
UCLASS(Blueprintable, BlueprintType, Config=Inventory, DefaultConfig)
class INVENTORY_API ATeachMan: public AInfo {
	GENERATED_BODY()
public:
#pragma region base
	ATeachMan();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  meta=(AdvancedDisplay, ForceAsFunction))
	void Init(UDataTable* Data);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  meta=(AdvancedDisplay, ForceAsFunction))
	void DeInit();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	bool Show(const FName& Id); // not const in case of delegates
	UFUNCTION(BlueprintCallable)
	void HideCurrent() {Hide(CurrentId);};
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm=Id))
	void Hide(const FName& Id = NAME_None);

	UPROPERTY(BlueprintReadWrite, Config)
	float Time = 30;

#pragma region delegates
	UPROPERTY(BlueprintReadWrite, Transient)
	FLearnShow OnShow;
	UPROPERTY(BlueprintReadWrite, Transient)
	FLearnHide OnHide;
#pragma endregion

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName CurrentId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};
