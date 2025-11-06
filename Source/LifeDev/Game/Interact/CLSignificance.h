// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "JSig/CSignificance.h"

#include "CLSignificance.generated.h"

class UCAnimator;

// LD base class for significance component.
// To handle significance stuff.
// Has support for animators.
// Will auto activate by default
UCLASS(ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLSignificance : public UCSignificance {
	GENERATED_BODY()

public:
	UCLSignificance();

	// will bind to one animator, other animators will be unbound.
	// don't call on CDO
	// beware of the IsOffIf* flags, since the animator might not re-activate,
	// and so this component might never reactivate, hence never becoming "on" again.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void BindAnim(UCAnimator* const Animator);

	// will bind to one animator
	UFUNCTION(BlueprintCallable)
	void UnbindAnim();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// cache of the bound anim
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UCAnimator> BoundAnim = nullptr;
};
