// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"
#include "JSig/CSignificance.h"

#include "CLSignificance.generated.h"

class UCAnimator;

UCLASS(ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLSignificance : public UCSignificance {
	GENERATED_BODY()

public:
	UCLSignificance();

	// will bind to one animator, other animators will be unbound ?
	UFUNCTION(BlueprintCallable)
	void BindAnim(UCAnimator* Animator);
	// will bind to one animator, other animators will be unbound ?
	UFUNCTION(BlueprintCallable)
	void UnbindAnim();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// cache of the bound anim
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UCAnimator* BoundAnim = nullptr;
};
