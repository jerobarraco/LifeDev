// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetDbgUI.generated.h"

class ULFeatsGroup;
class UScrollBox;

// baseclass debug stuff
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetDbgUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override;
	virtual void Load_Implementation() override;
	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> SBFlags;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatsGroup> FeatsDbg;
};
