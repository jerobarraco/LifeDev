// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "LQualitySwitchUI.generated.h"
class UCheckBox;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQualityChanged, int32, Q);

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULQualitySwitchUI : public UUserWidget {
	GENERATED_BODY()
	
public:
	ULQualitySwitchUI(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetQuality(int32 NewQuality = -1, bool Broadcast = false);
	virtual void SetQuality_Implementation(int32 NewQuality = -1, bool Broadcast = false);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetLabel(const FText& Text);
	virtual void SetLabel_Implementation(const FText& Text);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetQuality() { return Quality; };
	
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FOnQualityChanged OnChange;

protected:
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	TArray<UCheckBox*> CheckBoxes;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Label_T = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Quality = -1;
};
