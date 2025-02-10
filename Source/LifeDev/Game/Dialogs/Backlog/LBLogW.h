// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Diags/DiagTypes.h"

#include "LifeDev/Core/Settings/UI/LSetBaseUI.h"

#include "LBLogW.generated.h"

// currently shown on the settings menu, but later will be moved somewhere else maybe.

// a stub to show the backlog of dialogs.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULBLogW: public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override {};
	virtual void Load_Implementation() override {};
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void AddDiag(const FName Name, const FDialog& Diag);

	TArray<FName> Seen;
};
