// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "TeachTypes.h"

#include "TeachMan.generated.h"

enum class ETeachTarget : uint8;
class UFlags;
struct FTeachRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeachShow, const FName&, Id, const FTeachRow&, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeachHide, const FName&, Id);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(const FString, FTeachKeyName, const FString&, Key);

// for new users popups, and stuff.
UCLASS(Blueprintable, BlueprintType, Config=Inventory, DefaultConfig)
class INVENTORY_API ATeachMan: public AInfo {
	GENERATED_BODY()
public:
#pragma region base
	ATeachMan();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  meta=(AdvancedDisplay, ForceAsFunction))
	void Init();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  meta=(AdvancedDisplay, ForceAsFunction))
	void DeInit();
#pragma endregion

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	bool Has(const FName& Id) const;
	// default entry point to show a teach entry
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(AutoCreateRefTerm=Id))
	bool Show(const FName& Id); // not const in case of delegates
	// force show a teach entry skipping checks 
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm=Id, AdvancedDisplay))
	bool ShowNow(const FName& Id);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FORCEINLINE FName& GetCurrent() const { return CurrentId; }
	UFUNCTION(BlueprintCallable)
	void HideCurrent() { Hide(CurrentId); }
	// this is meant to be triggered even if the corresponding show was never called.
	// for example to mark that a player might not need the hint anymore.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(AutoCreateRefTerm=Id))
	void Hide(const FName Id = NAME_None); // intentionally not a ref
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(AutoCreateRefTerm=Id, AdvancedDisplay))
	void Set(const FName& Id = NAME_None) const;

	// first target added is going to be set as default
	UFUNCTION(BlueprintCallable)
	void AddTarget(const ETeachTarget Target, UDataTable* const InDT);
	UFUNCTION(BlueprintCallable)
	void SetTarget(const ETeachTarget Target);
	// pass the key names, that you use when formatting the data tables
	// e.g. if in the datatable you have "Press the {inventory_next} button", you need one entry with key "inventory_next"
	// and the value "
	// https://github.com/ibbles/LearningUnrealEngine/blob/master/Text%20and%20string%20formatting.md
	UFUNCTION(BlueprintCallable)
	void SetKeyNames(const ETeachTarget Target, const TMap<FString, FText> Names);
	
	// default duration for teach items. when the item time is <=0.
	UPROPERTY(BlueprintReadWrite, Config)
	float Time = 30;

#pragma region delegates
	UPROPERTY(BlueprintReadWrite, Transient)
	FTeachShow OnShow;
	UPROPERTY(BlueprintReadWrite, Transient)
	FTeachHide OnHide;
#pragma endregion

protected:
	UFUNCTION()
	void OnHardwareChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId);

	// todo maybe make virtual. maybe make settarget virtual.
	// todo reload text on change
	UFUNCTION()
	void InputChanged(const bool IsGP);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName CurrentId = NAME_None;
	// time at which the last hint was shown. as returned by World->GetTimeSeconds
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float LastTime = -1;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TMap<ETeachTarget, TObjectPtr<UDataTable>> DTs;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	TMap<ETeachTarget, FFormatNamedArguments> KeyArgs;

	ETeachTarget Tgt = ETeachTarget::DESK;

	FTimerHandle HShow;
};
