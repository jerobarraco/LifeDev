// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetDbgUI.generated.h"

class UJButton;
class UTextBlock;
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
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void DoTrace(const int32 Id);
	UFUNCTION()
	void FeatsDump(const int32 Id);
	UFUNCTION()
	void FlagsDump(const int32 Id) {};

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<ULFeatsGroup> FeatsDbg = nullptr;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> TFlags = nullptr;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BTrace = nullptr;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BFeatsDump = nullptr;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UJButton> BFlagsDump = nullptr;
};
