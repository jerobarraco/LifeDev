// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "GroupBox.generated.h"

class UDelegateWrapper;
class UCheckBox;

// internal don´t use https://forums.unrealengine.com/t/dynamic-multicast-delegate-how-to-bind-lambda/140046/13
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCBChange, class UCheckBox*, me, bool, IsChecked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGroupBoxChanged, int32, GroupId, int32, CheckId);

UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UGroupBox : public UUserWidget {
	GENERATED_BODY()
	
public:
	UGroupBox(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSelected(int32 NewSelected = -1, bool Broadcast = false);
	virtual void SetSelected_Implementation(int32 NewSelected = -1, bool Broadcast = false);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLabel(const FText& Text);
	virtual void SetLabel_Implementation(const FText& Text);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSelected() { return Selected; };
	
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FOnGroupBoxChanged OnChange;

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	int32 ID = -1;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void CheckSelected(UDelegateWrapper* W, int32 CID, UObject* CB);

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TArray<UCheckBox*> CheckBoxes;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	class UTextBlock* Label_T = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Selected = -1;

private: // nobody needs to know about this
	UPROPERTY(Transient)
	TArray<UDelegateWrapper*> Wrappers;
};
