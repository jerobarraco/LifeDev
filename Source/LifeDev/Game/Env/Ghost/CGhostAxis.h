// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only


#pragma once

#include "CoreMinimal.h"
#include "Interact/Animator/CAnimatorPID.h"

#include "CGhostAxis.generated.h"

class UCAnimator;
class UCRandomizer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCGhostAxis : public UCAnimatorPID {
	GENERATED_BODY()

public:
	UCGhostAxis();

	void RenameComp(const FString& Postfix);
protected:
#pragma region CDO
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCRandomizer* Rand = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimator* Anim = nullptr;
#pragma endregion 
};
