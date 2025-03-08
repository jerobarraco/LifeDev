// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GroupBox.h"

#include "LSaveGroup.generated.h"

class USlider;

// LD basic savefile group box
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSaveGroup : public UGroupBox {
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void LoadDoneAll();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<USlider> SL_Foxy = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Foxy = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool HasDoneSave = false;
	UPROPERTY(BlueprintReadWrite, Transient)
	bool LoadAllSlots = false;
};
