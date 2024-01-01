// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "LifeDev/Core/Settings/LSysSettings.h"

#include "LFeatCheck.generated.h"
/*
class UCheckBox;

// internal don´t use https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCBChange, class UCheckBox*, me, bool, IsChecked);

// look at JMiscUtils > DelegateWrapper
// i prefer not doing a generic lambda wrapper since i prefer this explicit way
// internal don´t use https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13
UCLASS()
class UCBChangeWrapper : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION()
	void Dispatch(bool IsChecked) {
		OnChange.Broadcast(CB, IsChecked);
	}
	UPROPERTY(Transient)
	FOnCBChange OnChange;
	UPROPERTY(Transient)
	UCheckBox* CB = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGroupBoxChanged, int32, id, int32, Q);
*/


class UTextBlock;
class ULSettings;

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API ULFeatCheck: public UUserWidget {
	GENERATED_BODY()

public:
	ULFeatCheck(const FObjectInitializer& O);

	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat);

protected:
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void FeatUpdate(EFeat Feat, bool bEnabled);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EFeat Feat = EFeat::NONE;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UCheckBox* Check = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Text = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettings* Settings = nullptr;
};
