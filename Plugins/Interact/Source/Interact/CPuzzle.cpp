// Copyright Jerónimo Barraco-Mármol

#include "CPuzzle.h"

#include "Interact.h"
#include "CInteract.h"
#include "DelegateWrappers.h"
#include "InteractAnim.h"

DEFINE_LOG_CATEGORY_STATIC(LogCPuzzle, Log, Log);

UCPuzzle::UCPuzzle(): Super() {
	// these 2 seems to work ok. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPuzzle::Reset_Implementation() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);
	for (AInteract* const I: Interacts) {
		if (!IsValid(I)) continue;
		I->Reset();
		I->SetEnabled(true);
	}

	// reset the solution and the objects
	ResetCurrents();
}

void UCPuzzle::SetInteracts(const TArray<AInteract*>& Inters) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);
	Unbind(); // unbind before emptying to make sure we don't remain subscribed to an orphan object.

	Interacts.Empty();
	for (AInteract* const I: Inters) {
		if (!IsValid(I)) continue;
		Interacts.Add(I);
	}

	Bind();
}

void UCPuzzle::Done(bool Ok) const {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);
	if (DisableOnDone) {
		for(AInteract* const I: Interacts) {
			I->SetEnabled(false);
		}
	}

	OnDone.Broadcast(Ok);
}

void UCPuzzle::ResetCurrents() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);
	
	CurrentIds.Empty(); // affects sequence and combo too
	if (Type == EPuzzleType::COMBINATION) {
		for (AInteract* const I: Interacts) {
			CurrentIds.Add(I->GetState()); // initialize to the current value. important since it could be different.
		}
		if (CurrentIds.Num()!=SolutionIDs.Num()) {
			UE_LOG(LogCPuzzle, Warning, TEXT("Current ids and Solution ids have different lenghts, the puzzle will not solve!"));
		}
	} else if (Type == EPuzzleType::SEQUENCE) {
		// not really need to be done each reset. but ... 
		for (AInteract* const I: Interacts) {
			AInteractAnim* const IA = Cast<AInteractAnim>(I);
			if (!IsValid(IA)) continue;
			// have to force it to not manage disabling, or it will break the puzzle potentially (re-enabling after anim)
			IA->DisableWhileAnim = false;
		}
	}
}

void UCPuzzle::Bind() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);

	int32 i = 0;
	for (AInteract* const I: Interacts) {
		if (!IsValid(I)) continue;

		UCInteract* const CI = I->GetComponentByClass<UCInteract>();
		if (!IsValid(CI)) continue;
		
		UDelegateWrapper* const Wrapper = NewObject<UDelegateWrapper>();
		if (!IsValid(Wrapper)) continue;
		Wrapper->Obj = I;
		Wrapper->ID = i;
		Wrapper->OnDispatch.AddUniqueDynamic(this, &UCPuzzle::InterTrigger);
		CI->OnTrigger.AddUniqueDynamic(Wrapper, &UDelegateWrapper::Dispatch);
		Wrappers.AddUnique(Wrapper);
		++i;
	}

	// done here so that on begin play it is also set
	ResetCurrents();
}

void UCPuzzle::BeginPlay() {
	Super::BeginPlay();
	Bind();
}

void UCPuzzle::Unbind() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs"), __func__);

	for (int32 i = 0; i<Interacts.Num() && i<Wrappers.Num(); ++i) {
		UDelegateWrapper* const W = Wrappers[i];
		if (!IsValid(W)) continue;
		
		AInteract* const I = Cast<AInteract>(W->Obj);
		if (!IsValid(I)) continue;
		
		UCInteract* const Comp = I->GetComponentByClass<UCInteract>();
		if (!IsValid(Comp)) continue;
		
		Comp->OnTrigger.RemoveAll(W);
	}

	Wrappers.Empty();
}

bool UCPuzzle::IsCurrentSolution() {
	if (CurrentIds.Num() != SolutionIDs.Num() ) return false;

	for (int32 i = 0; i< CurrentIds.Num(); ++i ) {
		if (CurrentIds[i] != SolutionIDs[i]) {
			UE_LOG(LogCPuzzle, Log, TEXT("Solution is different"));
			return false;
		}
	}

	UE_LOG(LogCPuzzle, Log, TEXT("Solution is correct"));
	return true;
}

bool UCPuzzle::CheckCombination(int32 ID) {
	if (ID<0 || ID>= CurrentIds.Num()) {
		UE_LOG(LogCPuzzle, Warning, TEXT("CheckCombination: ID out of bounds."));
		return false;
	}

	AInteract* const I = Interacts[ID];
	const int32 State = CurrentIds[ID] = I->GetState();
	UE_LOG(LogCPuzzle, Log, TEXT("%hs id=%i state=%i"), __func__, ID, State);

	// quite silly but.. who knows, maybe you want to have only one option for a combination.
	if (DisableOnInter) {
		I->SetEnabled(false);
	}

	return IsCurrentSolution();
}

bool UCPuzzle::CheckSequence(int32 ID) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs id=%i"), __func__, ID);

	if (ID<0 || ID>=Interacts.Num()) {
		UE_LOG(LogCPuzzle, Log, TEXT("%hs. Invalid id=%i"), __func__, ID);
		return false;
	}

	CurrentIds.Add(ID); // Allow to add repeated ones.
	if (DisableOnInter) {
		Interacts[ID]->SetEnabled(false);
	}
	return IsCurrentSolution();
}

void UCPuzzle::InterTrigger(UDelegateWrapper* Wrapper, int32 ID, UObject* Obj) {
	// trigger update now, before done
	OnUpdate.Broadcast();

	if (Type == EPuzzleType::SEQUENCE) {
		const bool Ok = CheckSequence(ID);
		// if the length matches return done anyways (means success false)
		if (SolutionIDs.Num() == CurrentIds.Num()) {
			Done(Ok);
			return;
		}
	} else if (Type == EPuzzleType::COMBINATION) {
		const bool Ok = CheckCombination(ID);
		if (Ok) {
			// only trigger when complete. combination can only be completed with ok.
			Done(true);
		}
	} else {
		UE_LOG(LogCPuzzle, Log, TEXT("Invalid puzzle type"));
	}
}

void UCPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Super::EndPlay(EndPlayReason);
}
