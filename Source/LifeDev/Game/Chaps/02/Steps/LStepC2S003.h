// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "Diags/DiagTypes.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC2S003.generated.h"

// step when tape is picked. contains the dialog interaction.
// finishes with dialog.
UCLASS(Blueprintable, BlueprintType)
class ALStepC2S003 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC2S003();

protected:
	virtual void Start_Implementation() override;

	UFUNCTION()
	void DlgShown(const FDialog& Diag);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	USoundBase* SFX_Analog = nullptr;
};
