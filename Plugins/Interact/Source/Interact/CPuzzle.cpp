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
	if (Ok && DisableOnDone) {
		for(UCInteract* const I: Interacts) {
			I->SetEnabled(false);
		}
	}
	OnDone.Broadcast(Ok);
}

bool UCPuzzle::CheckCombination(int32 ID) {
	return false;
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

bool UCPuzzle::CheckSequence(int32 ID) {
	UE_LOG(LogCPuzzle, Log, TEXT("Toggling id=%i"), ID);

	if (CurrentIds.Contains(ID)) {
		CurrentIds.Remove(ID);
	} else {
		CurrentIds.Add(ID);
	}

	if (CurrentIds.Num() != SequenceIDs.Num() ) return false;
	
	for (int32 i = 0; i< CurrentIds.Num(); ++i ) {
		if (CurrentIds[i]!=SequenceIDs[i]) {
			UE_LOG(LogCPuzzle, Log, TEXT("Sequence is different"));
			return false;
		}
	}

	UE_LOG(LogCPuzzle, Log, TEXT("Sequence is correct"));
	return true;
}

void UCPuzzle::InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj) {
	// trigger update now, before done
	OnUpdate.Broadcast();

	if (Type == EPuzzleType::SEQUENCE) {
		const bool Ok = CheckSequence(ID);
		// if the length matches return done anyways
		if (SequenceIDs.Num() == CurrentIds.Num()) {
			Done(Ok);
			return;
		}
	} else if (Type == EPuzzleType::COMBINATION) {
		const bool Ok = CheckCombination(ID);
	} else {
		UE_LOG(LogCPuzzle, Log, TEXT("Invalid puzzle type"));
	}
}

void UCPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Super::EndPlay(EndPlayReason);
}
