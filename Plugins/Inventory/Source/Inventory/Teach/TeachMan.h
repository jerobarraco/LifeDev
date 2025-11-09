// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "TeachTypes.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "TeachMan.generated.h"

class UFlags;
struct FTeachRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTeachShow, const FName&, Id, const FTeachRow&, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeachHide, const FName&, Id);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(const FString, FTeachKeyName, const FString&, Key);

// for new users popups.
// to show buttons, on your DT you need to use {}.
// the value inside would be the "Key" defined by SetKeyNames. the text is the value.
// A way to obtain this, is to iterate through all the IMCs, then iterate over each Mapping,
// On each mapping get the PlayerMappableKeySettings (you need to set it on the imcs)
// And use the "Name" field as the key. You can add a suffix to distinguish each target, since these names have to be unique.
// Before passing to SetKeyNames, you would need to remove that suffix.
// The text can be obtained through Mapping.Key.GetDisplayName.
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
	
	UFUNCTION(BlueprintCallable)
	void SetDT(UDataTable* const InDT);
	UFUNCTION(BlueprintCallable)
	void SetTarget(const EInputType Target);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EInputType GetTarget() const { return Tgt; }

	// pass the key names, that you use when formatting the data tables
	// e.g. if in the datatable you have "Press the {inventory_next} button", you need one entry with key "inventory_next"
	// and the value "X".
	// https://github.com/ibbles/LearningUnrealEngine/blob/master/Text%20and%20string%20formatting.md
	UFUNCTION(BlueprintCallable)
	void SetKeyNames(const EInputType Target, const TMap<FString, FText> Names);
	// later. this could live in a subsystem if i wanted to use elsewhere. or maybe just keep using the teach.

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

	UFUNCTION()
	void InputChanged(const EInputType Tgt);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FName CurrentId = NAME_None;
	// time at which the last hint was shown. as returned by World->GetTimeSeconds
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float LastTime = -1;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDataTable> DT = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TMap<EInputType, TObjectPtr<UDataTable>> DTs;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	
	TMap<EInputType, FFormatNamedArguments> KeyArgs; // key names for format
	EInputType Tgt = EInputType::DESK; // current target

	FTimerHandle HShow;
};
