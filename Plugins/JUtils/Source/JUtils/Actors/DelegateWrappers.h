// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "DelegateWrappers.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWrapperID, class UDelegateWrapperID*, Wrapper, int32, ID, UObject*, Obj);

// https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13
// A simple wrapper for binding to delegates with extra parameters.
// To use:
// UDelegateWrapperID* const Wrapper = NewObject<UDelegateWrapperID>();
// Wrapper->ID = WhateverIDGen();
// Wrapper->OnDispatch.AddUniqueDynamic(this, &UGroupBox::ResetSelected);
// C->OnCheckStateChanged.AddUniqueDynamic(Wrapper, &UCBChangeWrapper::DispatchBool);
UCLASS(Blueprintable, BlueprintType)
class UDelegateWrapperID : public UObject {
	GENERATED_BODY()

public:
	// bind this function to the other delegate
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Dispatch() { OnDispatch.Broadcast(this, ID, Obj); }

	// or bind this function to the other delegate
	UFUNCTION(BlueprintCallable)
	void DispatchBool(bool IsChecked) { Dispatch(); };
	
	// Set this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category=SetUp)
	int32 ID = -1;

	// Set this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category=SetUp)
	UObject* Obj = nullptr;
	
	// subscribe to this 
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient)
	FOnWrapperID OnDispatch;
};
