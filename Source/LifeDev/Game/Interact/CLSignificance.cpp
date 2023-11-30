// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLSignificance.h"

#include "Interact/Animator/CAnimator.h"

UCLSignificance::UCLSignificance() :Super(){ }

void UCLSignificance::BindAnim(UCAnimator* Animator) {
	UnbindAnim();
	if (!IsValid(Animator)) return;

	BoundAnim = Animator;
	Comps.AddUnique(BoundAnim);
	BoundAnim->OnBegin.AddUniqueDynamic(this, &UCLSignificance::ActivateNow);
	BoundAnim->OnEnd.AddUniqueDynamic(this, &UCLSignificance::Deactivate);
}

void UCLSignificance::UnbindAnim() {
	if (IsValid(BoundAnim)) {
		BoundAnim->OnBegin.RemoveAll(this);
		BoundAnim->OnEnd.RemoveAll(this);
	}
	Comps.Remove(BoundAnim);
	BoundAnim = nullptr;
}

void UCLSignificance::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UnbindAnim();
	Super::EndPlay(EndPlayReason);
}


