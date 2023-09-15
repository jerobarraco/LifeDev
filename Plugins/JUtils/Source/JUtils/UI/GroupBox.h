// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "GroupBox.generated.h"
class UCheckBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGroupBoxChanged, class UGroupBox*, me, int32, Q);

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

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void ResetSelected(bool bIsChecked);

	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TArray<UCheckBox*> CheckBoxes;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Label_T = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Selected = -1;
};
