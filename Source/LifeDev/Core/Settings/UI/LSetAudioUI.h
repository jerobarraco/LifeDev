// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "JUtils/UI/BaseUI.h"

#include "LSetAudioUI.generated.h"

// WIP moving the video stuff from the settingsui
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetAudioUI : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction))
	void Apply();
	virtual void Apply_Implementation(){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction))
	void Load();
	virtual void Load_Implementation(){}

protected:
};
