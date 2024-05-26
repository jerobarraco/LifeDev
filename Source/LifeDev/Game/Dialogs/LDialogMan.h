// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Diags/DiagMan.h"
#include "Diags/DiagTypes.h"

#include "LDialogMan.generated.h"

// Dialog manager. dynamically instanced on the level
UCLASS(Blueprintable)
class LIFEDEV_API ALDialogMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDialogMan();

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void DiagShown(const FDialog& Diag);
};