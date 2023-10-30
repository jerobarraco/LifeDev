// Copyright Jerónimo Barraco-Mármol

#include "CPuzzle.h"

#include "DelegateWrappers.h"


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

void UCPuzzle::Bind() {
	int32 i = 0;
	for (UCInteract* const I: Interacts) {
		UDelegateWrapper* const Wrapper = NewObject<UDelegateWrapper>();
		if (!IsValid(Wrapper)) continue;
		Wrapper->Obj = I;
		Wrapper->ID = i;
		Wrapper->OnDispatch.AddUniqueDynamic(this, &UCPuzzle::InterTrigger);
		I->OnTrigger.AddUniqueDynamic(Wrapper, &UDelegateWrapper::Dispatch);
		++i;
	}
}

void UCPuzzle::BeginPlay() {
	Super::BeginPlay();
	Bind();
}

void UCPuzzle::Unbind() {
	for (UCInteract* const I: Interacts) {
		I->OnTrigger.RemoveAll(this);
	}
}

void UCPuzzle::InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj) {
	// Verify conditions
	// TODO check solution
	OnUpdate.Broadcast();
	const bool Ok = true;
	if (Ok) {
		Done();
	}
}

void UCPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Super::EndPlay(EndPlayReason);
}
