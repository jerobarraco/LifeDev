// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetSoundUI.generated.h"

enum class EFeat : uint8;
class ULFeatsGroup;

// base class for the sound settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetSoundUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;

protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatsGroup> FeatsGroup;

	TMap<EFeat, FText> FeatTexts;
};
