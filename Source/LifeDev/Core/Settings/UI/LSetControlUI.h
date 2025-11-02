// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetControlUI.generated.h"

class ULInputSelector;
class UJButton;
// baseclass for the control settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetControlUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override {}
	virtual void Load_Implementation() override {}

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void SetDefaults(const int32 Id);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TObjectPtr<ULInputSelector>> Selectors;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BtnDefaults = nullptr;
	
};
