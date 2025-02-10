// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "JUtils/UI/BaseUI.h"

#include "LSettingsUI.generated.h"

class UGroupBox;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettingsUI : public UBaseUI {
	GENERATED_BODY()

public:
	ULSettingsUI();

	// will reload the settings
	virtual void Show_Implementation() override;
	virtual void Hide_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction, ForceAsFunction))
	void Apply();
	virtual void Apply_Implementation(){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta =(UnsafeDuringActorConstruction, ForceAsFunction))
	void Load();
	virtual void Load_Implementation(){}

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
};
