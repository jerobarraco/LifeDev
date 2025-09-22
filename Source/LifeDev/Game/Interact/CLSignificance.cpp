// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLSignificance.h"

#include "Interact/Animator/CAnimator.h"

UCLSignificance::UCLSignificance() {
	TickIntervals[ESigValue::High] = 1/60.;
	TickIntervals[ESigValue::Med] = 1/30.;
	TickIntervals[ESigValue::Low] = 1/20.;
}

void UCLSignificance::BindAnim(UCAnimator* const Animator) {
	UnbindAnim();
	if (UNLIKELY(!IsValid(Animator))) return;

	BoundAnim = Animator;
	CompsTicks.AddUnique(BoundAnim);
	BoundAnim->OnBegin.AddUniqueDynamic(this, &UCLSignificance::ActivateNow);
	// can't mark this as ufunction, it's virtual, the parent is ufunction. it works.
	// TODO test if it actually does work.
	BoundAnim->OnEnd.AddUniqueDynamic(this, &UCLSignificance::Deactivate);
}

void UCLSignificance::UnbindAnim() {
	if (LIKELY(IsValid(BoundAnim))) {
		BoundAnim->OnBegin.RemoveAll(this);
		BoundAnim->OnEnd.RemoveAll(this);
	}

	CompsTicks.Remove(BoundAnim);
	BoundAnim = nullptr;
}

void UCLSignificance::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UnbindAnim();
	Super::EndPlay(EndPlayReason);
}
