// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LifeDev/Game/Dialogs/LDialogUI.h"

#include "LBLogItemW.generated.h"

// currently shown on the settings menu, but later will be moved somewhere else maybe.

class UTextBlock;
class URichTextBlock;
// a stub to show the backlog of dialogs.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULBLogItemW : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetUp(const FDiag& Diag);
	// don't const or ue will make it pure. wtf

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> T_Name = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(BindWidgetOptional))
	TObjectPtr<URichTextBlock> R_Diag = nullptr;
};
