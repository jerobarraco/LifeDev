// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GroupBox.h"

#include "LSaveGroup.generated.h"

class USlider;
// LD basic savefile box
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSaveGroup : public UGroupBox {
	GENERATED_BODY()

public:

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<USlider> SL_Foxy = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> T_Foxy = nullptr;
};
