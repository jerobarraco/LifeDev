// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

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
	virtual void Activate(const bool bReset) override;
	virtual void Deactivate() override;

#pragma region CDO
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCRandomizer> Rand = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimator> Anim = nullptr;
#pragma endregion
};
