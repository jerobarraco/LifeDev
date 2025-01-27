// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "DelegateWrappers.generated.h"

class UDelegateWrapper;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWrapperID, UDelegateWrapper* const, Wrapper, int32, ID, UObject* const, Obj);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnWrapperIDC, UDelegateWrapper* const, int32, UObject* const);

// Note, take a look at FStreamableDelegate
// Delegate with parameters we need once the asset had been loaded such as the Id we loaded and the location to spawn at. Will call function 'OnMonsterLoaded' once it's complete.
// FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, MonsterId, SpawnLocation);
// The actual async load request
// Manager->LoadPrimaryAsset(MonsterId, Bundles, Delegate);

// A simple wrapper for binding to delegates with extra parameters.
// To use:
// UDelegateWrapperID* const Wrapper = NewObject<UDelegateWrapperID>();
// Wrapper->ID = whatever your id (or don't set)
// Wrapper->Obj = whatever object to associate (or null)
// Wrapper->OnDispatch.AddUniqueDynamic(this, &UGroupBox::ResetSelected);
// C->OnCheckStateChanged.AddUniqueDynamic(Wrapper, &UCBChangeWrapper::DispatchBool);
// Also you NEED to keep a ref to the Wrapper, or it will get GarbageCollected.
// https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13?u=nande
// https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/15?u=nande
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UDelegateWrapper : public UObject {
	GENERATED_BODY()

public:
	// bind this function to the other delegate
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Dispatch() {
		OnDispatch.Broadcast(this, ID, Obj.Get());
		OnDispatchCPP.Broadcast(this, ID, Obj.Get());
	}

	// or bind this function to the other delegate (useful when the other delegate has a param)
	UFUNCTION(BlueprintCallable)
	void DispatchBool(bool IsChecked) { Dispatch(); }

	// the id for this wrapper. passed on OnDispatch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 ID = -1;

	// the obj (if any) associated to this wrapper. passed on OnDispatch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UObject> Obj = nullptr;

	// subscribe to this
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient)
	FOnWrapperID OnDispatch;

	// subscribe to this if you need a lambda λ
	FOnWrapperIDC OnDispatchCPP;
};
