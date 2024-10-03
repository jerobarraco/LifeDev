// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "LSetBaseUI.h"

#include "LSetAudioUI.generated.h"

// WIP moving the video stuff from the settingsui
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetAudioUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override {};

	virtual void Load_Implementation() override {};

protected:
};
