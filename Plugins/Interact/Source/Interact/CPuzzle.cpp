// Copyright Jerónimo Barraco-Mármol

#include "CPuzzle.h"


DEFINE_LOG_CATEGORY_STATIC(LogCPuzzle, Log, Log);

UCPuzzle::UCPuzzle(): Super() {
	// these 2 seems to work ok. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPuzzle::SetInteracts(const TArray<UCInteract*>& Inters) {
	Unbind();

	Interacts.Empty();
	for (UCInteract* const I: Inters) {
		if (IsValid(I)) {
			Interacts.Add(I);
		}
	}

	Bind();
}

void UCPuzzle::Done(bool Ok) const {
	OnDone.Broadcast(Ok);
}

void UCPuzzle::Triggered() {
	// TODO check solution
	OnUpdate.Broadcast();
	const bool Ok = true;
	if (Ok) {
		Done();
	}
}

void UCPuzzle::Bind() {
	for (UCInteract* const I: Interacts) {
		// TODO add wrapper
		I->OnTrigger.AddUniqueDynamic(this, &UCPuzzle::Triggered);
	}
}

void UCPuzzle::BeginPlay() {
	Super::BeginPlay();
	Bind();
}

void UCPuzzle::Unbind() {
	for (UCInteract* const I: Interacts) {
		// TODO add wrapper
		I->OnTrigger.RemoveAll(this);
	}
}

void UCPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Super::EndPlay(EndPlayReason);
}
