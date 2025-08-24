// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "CPuzzle.h"

#include "DelegateWrappers.h"
#include "Eval.h"

#include "Animator/CAnimatorMix.h"
#include "Interact.h"
#include "InteractAnim.h"

DEFINE_LOG_CATEGORY_STATIC(LogCPuzzle, Log, Log);

UCPuzzle::UCPuzzle() {
	// seems to work. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPuzzle::BeginPlay() {
	Super::BeginPlay();
	Bind();
}

void UCPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unbind();
	Super::EndPlay(EndPlayReason);
}

void UCPuzzle::Reset_Implementation() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));

	for (TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->Reset();
		I->SetActive(true);
	}

	// reset the solution and the objects
	ResetCurrents();

	OnReset.Broadcast();
}

void UCPuzzle::SetInteracts(const TArray<AInteract*>& Inters) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));
	Unbind(); // unbind before emptying to make sure we don't remain subscribed to an orphan object.

	Pieces.Empty(Inters.Num());
	for (AInteract* const I: Inters) {
		if (UNLIKELY(!IsValid(I))) continue;
		Pieces.Add(I);
	}

	Bind();
}

void UCPuzzle::ResetCurrents() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs, o=%s"), __func__, *GetNameSafe(this));
	
	CurrentIds.Empty(); // affects sequence and combo too
	if (Type == EPuzzleType::COMBINATION) {
		for (const TSoftObjectPtr<AInteract>& I: Pieces) {
			if (UNLIKELY(!I.IsValid())) continue;
			CurrentIds.Add(I->GetState()); // initialize to the current value. important since it could be different.
		}
		if (CurrentIds.Num() != Solution.Num()) {
			UE_LOG(LogCPuzzle, Warning, TEXT("Current ids and Solution ids have different lenghts, the puzzle will not solve!"));
		}
	} else if (Type == EPuzzleType::SEQUENCE) {
	}
}

void UCPuzzle::Bind() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));

	int32 i = 0;
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;

		UDelegateWrapper* const Wrapper = NewObject<UDelegateWrapper>();
		if (UNLIKELY(!IsValid(Wrapper))) continue;

		Wrapper->Obj = I.Get();
		Wrapper->ID = i;
		Wrapper->OnDispatch.AddUniqueDynamic(this, &UCPuzzle::InterTrigger);

		// bind to the Interact when triggered. if it's animated wait for it to end.
		// AInteractAnim* const IA = Cast<AInteractAnim>(I);
		// if (IsValid(IA))
			// IA->OnAnimEnd.AddUniqueDynamic(Wrapper, &UDelegateWrapper::Dispatch);
		// else
		I->OnTrigger.AddUniqueDynamic(Wrapper, &UDelegateWrapper::Dispatch);

		Wrappers.AddUnique(Wrapper);
		++i;
	}

	// done here so that on begin play it is also set
	ResetCurrents();

	// Disable anim.
	// have to force it to not manage disabling, or it will break the puzzle potentially (re-enabling after done).
	SetDisableWhileAnims(false);
}

void UCPuzzle::Unbind() {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(this));

	for (const UDelegateWrapper* const W: Wrappers){
		if (UNLIKELY(!IsValid(W))) continue;
		
		AInteract* const I = Cast<AInteract>(W->Obj);
		if (UNLIKELY(!IsValid(I))) continue;
		I->OnTrigger.RemoveAll(W);
	}

	Wrappers.Empty();
}

bool UCPuzzle::IsCurrentSolution() {
	const int32 IdsNum = CurrentIds.Num();
	if (UNLIKELY(IdsNum != Solution.Num())) {
		UE_LOG(LogCPuzzle, Warning, TEXT("%hs Length of Solution is different from length of Interacts. Stop."), __func__);
		return false;
	}

	for (int32 i = 0; i< IdsNum; ++i ) {
		if (CurrentIds[i] != Solution[i]) {
			UE_LOG(LogCPuzzle, Log, TEXT("%hs Solution is different"), __func__);
			return false;
		}
	}

	UE_LOG(LogCPuzzle, Log, TEXT("%hs Solution is correct"), __func__);
	return true;
}

bool UCPuzzle::CheckCombination(const int32 ID) {
	if (UNLIKELY(ID<0 || ID>= CurrentIds.Num())) {
		UE_LOG(LogCPuzzle, Warning, TEXT("CheckCombination: ID out of bounds."));
		return false;
	}

	AInteract* const I = Pieces[ID].Get();
	if (UNLIKELY(!IsValid(I))) {
		UE_LOG(LogCPuzzle, Warning, TEXT("%hs Interact is invalid. ID=%i"), __func__, ID);
		return false;
	}

	const int32 State = CurrentIds[ID] = I->GetState();
	UE_LOG(LogCPuzzle, Log, TEXT("%hs id=%i state=%i"), __func__, ID, State);

	return IsCurrentSolution();
}

bool UCPuzzle::CheckSequence(const int32 ID) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs id=%i"), __func__, ID);

	if (UNLIKELY(ID<0 || ID>=Pieces.Num())) {
		UE_LOG(LogCPuzzle, Log, TEXT("%hs. Invalid id=%i"), __func__, ID);
		return false;
	}

	CurrentIds.Add(ID); // Allow to add repeated ones.

	return IsCurrentSolution();
}

bool UCPuzzle::CheckCondition() const {
	// eval is going to check anyway. and Res>0 will be false. this way this condition is not compiled on shipping
	UE_CLOG(UNLIKELY(Condition.IsEmpty()), LogCPuzzle, Warning,
		TEXT("%hs CPuzzle is set to condition, but condition is empty."
		" This won't work. Stop."),
		__func__);

	const UEval* const Eval = UEval::Instance(this);
	if (UNLIKELY(!Eval)) return false;

	double Res;
	if (!Eval->Eval(Condition, Res)) return false;

	return Res > 0;
}

void UCPuzzle::InterTrigger(UDelegateWrapper* const Wrapper, const int32 ID, UObject* const Obj) {
	// this code sucks.
	bool IsOk = false;
	bool IsDone = true;
	if (Type == EPuzzleType::SEQUENCE) {
		IsOk = CheckSequence(ID);
		// don't trigger Done if we haven't reached, or pass the solution size.
		IsDone = CurrentIds.Num() >= Solution.Num();
	} else if (Type == EPuzzleType::COMBINATION) {
		IsOk = CheckCombination(ID);
	} else if (Type == EPuzzleType::CONDITION) {
		IsOk = CheckCondition();
	} else {
		UE_LOG(LogCPuzzle, Log, TEXT("InterTrigger: Invalid puzzle type."));
		return;
	}

	// the flags and order is like this to allow autodiags and other clients to test the currentIds before PreDone is called.
	// trigger update now! before done.
	// important for APuzzle timer and for logical order in the flow
	OnUpdate.Broadcast();

	if (LIKELY(IsDone)) PreDone(IsOk); // pre-done checks IsOk inside. so it's safe.
}

void UCPuzzle::PreDone(const bool Ok) const {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs. ok=%i o=%s"),
		__func__, Ok, *GetNameSafe(this->GetOwner()));

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	if (!Ok) {
		Done(Ok);
		return;
	}

	// the important part of this code is to disable interactions to fix the issue with the user toggling another piece
	// when the puzzle has already been solved. hence, this is not necessary to be exposed to children or clients.
	if (DisableOnDone) {
		SetDisableWhileAnims(false); // force to avoid issues.
		SetActives(false);
	}

	// call Done now or delayed if it's animating.
	bool Animating = false;
	float Time = 0;
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		const AInteractAnim* const IA = Cast<AInteractAnim>(I.Get());
		if (UNLIKELY(!IsValid(IA))) continue;
		
		const UCAnimatorMix* const Anim = IA->GetAnim();
		if (UNLIKELY(!Anim)) continue;

		if (LIKELY(!Anim->IsActive())) continue;

		Animating = true;
		// It's ok to grab the duration because PreDone is called as a direct side effect of the trigger.
		// Hence, Elapsed ~= 0
		Time = FMath::Max(Time, IA->GetAnim()->Duration);
	}
	
	if (Animating) { // wait for animation
		FTimerHandle H;
		auto C = [this, Ok] { Done(Ok); };
		World->GetTimerManager().SetTimer(H, C, Time, false);
	} else
		Done(Ok);
}

void UCPuzzle::Done(const bool Ok) const {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs. ok=%i o=%s"),
		__func__, Ok, *GetNameSafe(this->GetOwner()));
	OnDone.Broadcast(Ok);
}

void UCPuzzle::SetDisableWhileAnims(const bool NewDisable) const {
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		AInteractAnim* const IA = Cast<AInteractAnim>(I.Get());
		if (UNLIKELY(!IsValid(IA))) continue;
		// have to force it to not manage disabling, or it will break the puzzle potentially (re-enabling after anim)
		IA->DisableWhileAnim = NewDisable;
	}
}

void UCPuzzle::SetActives(const bool NewActive) const {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs, o=%s newActive=%i"),
		__func__, *GetNameSafe(GetOwner()), NewActive);

	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->SetActive(NewActive);
	}
}

void UCPuzzle::SetAutoActives(const bool NewAutoActive) const {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs, o=%s NewAutoActives=%i"),
		__func__, *GetNameSafe(GetOwner()), NewAutoActive);

	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->UseAutoActivate = NewAutoActive;
	}
}

void UCPuzzle::SetStates(const TArray<int32>& States) {
	const int32 Num = States.Num();
	const int32 Num2 = Pieces.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Pieces[i].Get();
		if (UNLIKELY(!IsValid(I))) continue;
		I->SetState(States[i]);
	}

	// reload the ids if needed. important.
	ResetCurrents();
}

void UCPuzzle::SetLocks(const TArray<bool>& Locks) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs, o=%s"),
		__func__, *GetNameSafe(GetOwner()));
	const int32 Num = Locks.Num();
	const int32 Num2 = Pieces.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Pieces[i].Get();
		if (UNLIKELY(!IsValid(I))) continue;
		I->IsLocked = Locks[i];
	}
}

void UCPuzzle::SetHiddensInGame(const bool NewHidden) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs, o=%s newHidden=%i"),
		__func__, *GetNameSafe(GetOwner()), NewHidden);
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->SetActorHiddenInGame(NewHidden);
	}
}

void UCPuzzle::SetUseHints(const bool NewHint) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs Obj=%s"), __func__, *GetNameSafe(GetOwner()));
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->SetUseHint(NewHint);
		// I->UseHint = NewHint;
	}
}

void UCPuzzle::SetHintConditions(const FString& Cond) {
	UE_LOG(LogCPuzzle, Log, TEXT("%hs Obj=%s"), __func__, *GetNameSafe(GetOwner()));
	for (const TSoftObjectPtr<AInteract>& I: Pieces) {
		if (UNLIKELY(!I.IsValid())) continue;
		I->HintCondition = Cond;
	}
}

// void UCPuzzle::ShowHint() {
// 	UE_LOG(LogCPuzzle, Log, TEXT("%hs Obj=%s"), __func__, *GetNameSafe(GetOwner()));
// 	for (AInteract* const I: Interacts) {
// 		if (UNLIKELY(!IsValid(I))) continue;
// 		I->UseHint = true; // reset in case it got de-Hint on trigger.
// 		I->ShowHint();
// 	}
// }
