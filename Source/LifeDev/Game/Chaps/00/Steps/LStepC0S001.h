// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC0S001.generated.h"

class UCQuickMesh;
// chapter 0: the step where the char is in bed berating
UCLASS(Blueprintable, BlueprintType)
class ALStepC0S001 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC0S001();

protected:
	virtual void Start_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> Target = nullptr;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> TargetArrow = nullptr;
#endif
};
