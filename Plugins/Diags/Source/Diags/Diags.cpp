// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Diags.h"

#include "Eval.h"
#include "JUtils/Misc/JUtilsMisc.h"

DEFINE_LOG_CATEGORY_STATIC(LogDiags, Log, Log);

UDiags* UDiags::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UDiags* const D = W->GetSubsystem<UDiags>();
	return UNLIKELY(IsValid(D)) ? D : nullptr;
}

void UDiags::AddDiag(const FDiag& Diag) {
	Pending.Add(Diag);
	ShowNext();
}

bool UDiags::AddDiagId(const FName& Row) {
	UE_LOG(LogDiags, Log, TEXT("%hs: row=%s"), __func__, *Row.ToString());
	FDiag Diag;
	const bool Ok = GetDiag(Row, Diag);
	if (UNLIKELY(!Ok)) return false;

	double Res = 0;
	const bool Cond = CheckCondition(Diag.Condition, Res);
	if (UNLIKELY(!Cond)) {
		UE_LOG(LogDiags, Log, TEXT("%hs: Condition not met. row=%s condition=%s"),
			__func__, *Row.ToString(), *Diag.Condition);
		return false;
	}

	OnAdd.Broadcast(Row, Diag); // before addDiag since it will trigger all sorts of other stuff.
	AddDiag(Diag);
	return true;
}

bool UDiags::AddId(const FName& Row) {
	UE_LOG(LogDiags, Log, TEXT("%hs: id='%s'"), __func__, *Row.ToString());
	if (UNLIKELY(Row.IsNone())) return false;

	// attempt to add a sequence (can be random) (could trigger another call to AddId)
	if (AddGroupId(Row)) return true;

	// otherwise attempt a dialog
	// not using likely due to auto-dialogs spamming the subsystem.
	if (AddDiagId(Row)) return true;

	// this also would capture a sequence that is empty or the ids are none.
	UE_CLOG(UseWarning, LogDiags, Warning,
		TEXT("%hs: Could not find dialog nor group with the id=%s. "
			"Or the group was empty or invalid. Or it didn't pass it's condition."), __func__, *Row.ToString());
	return false;
}

bool UDiags::AddIdMany(const TArray<FName>& Rows) {
	const int32 Num = Rows.Num();
	bool Success = false; // return false if nothing was added
	for (int32 i = 0; i < Num; ++i) {
		// done this way on purpose, so if a sequence adds another sequence
		// it will get "expanded" as a parenthesis.
		// AddId is safe to call many times since ShowNext has a flag
		const FName& Row = Rows[i];
		// notice this is recursive. that's on purpose, but be careful.
		const bool Ok = AddId(Row);

		// now that we have conditions, some dialogs might fail, and we'll be fine with that.
		Success = Success | Ok;
	}

	return Success;
}

bool UDiags::AddGroup(const FDiagGroup& Seq) {
	const TArray<FName>& Rows = Seq.DiagRows;
	const int32 Num = Rows.Num();
	if (Num <= 0) return false;

	double Res;
	const bool CondOk = CheckCondition(Seq.Condition, Res);

	// these two use the condition to select the dialog
	if (Seq.Type == EGroupType::PICK_LOOP) {
		const int32 i = FMath::Max(0, FMath::RoundToZero(FMath::Modulo(Res, Num)));
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	}
	
	if (Seq.Type == EGroupType::PICK_CLAMP) {
		const int32 i = FMath::Clamp(Res, 0, Num-1);
		const FName DiagRow = Rows[i];
		return AddId(DiagRow);
	}

	// the rest use the condition to pass
	if (UNLIKELY(!CondOk)) return false;
	
	if (Seq.Type == EGroupType::SEQUENCE) {
		return AddIdMany(Rows);
	}

	if (Seq.Type == EGroupType::MATCH) {
		for (const FName& N: Rows) {
			if (AddId(N)) return true;
		}
		return false;
	}

	if (Seq.Type == EGroupType::RANDOM) {
		TArray<FName> Shuffled = Rows; // make a copy
		UJUtilsMisc::ArrayShuffle<FName>(Shuffled);
		for (const FName& N: Shuffled) {
			if (AddId(N)) return true;
		}
		return false;
	}
	
	return false;
}

bool UDiags::AddGroupId(const FName& RowName) {
	FDiagGroup Seq;
	const bool Ok = GetGroup(RowName, Seq);
	if (!Ok) return false;

	const int32 DiagNum = Seq.DiagRows.Num();
	const FString RowNameStr = RowName.ToString();
	// prevent recursion. Notice this doesn't fix cyclic sequences. no simple way to tell either.
	// not a priority either.
	// int because num-1 can be negative. iterating backwards to be able to remove easily.
	for (int32 i = DiagNum -1; i>=0; --i) {
		const FName& DiagName = Seq.DiagRows[i];
		if (LIKELY(DiagName != RowName)) continue;

		UE_LOG(LogDiags, Warning, TEXT("Attempted to add a recursive sequence. Seq=%s diag=%s"),
			*RowNameStr, *DiagName.ToString());
		// this is safe only because GetSeq returns a copy. :) Not using Swap due to iteration
		// not shrinking to improve perf. i don't really care about that here. that's a temporary array. 
		Seq.DiagRows.RemoveAt(i, EAllowShrinking::No);
	}

	if (DiagNum < 1) {
		UE_LOG(LogDiags, Warning, TEXT("%hs: Sequence is empty! (after removing loops). Skip."
			" row=%s"), __func__, *RowName.ToString());
		return false;
	}

	return AddGroup(Seq);
}

void UDiags::SetEffect(const FName& Name, const bool Enable) {
	// todo consider moving the handling of effects and keeping track of, to another more general place
	// it's also used by lstep, but i can't set it on the step plugin since it would have to depend on 
	// this module, and i don't want that.
	// maybe jutils?
	if (Effects.Contains(Name) == Enable) return; // no change

	if (Enable) {
		Effects.Add(Name);
	} else {
		Effects.RemoveSwap(Name);
	}

	OnEffect.Broadcast(Name, Enable);
}

void UDiags::DiagDone() {
	IsShowing = false;
	ShowNext();
}

void UDiags::Init() {
	Eval = UEval::Instance(this);
}

void UDiags::DeInit() {
	Chars = nullptr;
	Eval = nullptr;
	Diags.Empty();
	Groups.Empty();
}

bool UDiags::GetChar(const FName& RowName, FDiagChar& OutChar) const {
	if (UNLIKELY(RowName.IsNone())) return false;
	if (UNLIKELY(!IsValid(Chars))) return false;

	const FDiagChar* const Row = Chars->FindRow<FDiagChar>(RowName, TEXT(""), UseWarning);
	if (UNLIKELY(!Row)) {
		UE_LOG(LogDiags, Warning, TEXT("%hs Could not find character for row=%s"), __func__, *RowName.ToString());
		return false;
	}

	OutChar = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer.
	return true;
}

bool UDiags::GetDiag(const FName& RowName, FDiag& OutRow) const {
	if (UNLIKELY(RowName.IsNone())) return false;

	// iterate in reverse to allow to override stuff. but make add quicker.
	for (int32 i = Diags.Num()-1; i>=0; --i ) {
		UDataTable* const DT = Diags[i];
		if (UNLIKELY(!IsValid(DT))) continue;

		const FDiag* const Row =
			DT->FindRow<FDiag>(RowName, TEXT(""), UseWarning);
		if (UNLIKELY(!Row)) continue;

		OutRow = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer.
		return true;
	}

	UE_LOG(LogDiags, Verbose, TEXT("%hs Could not find dialog for row=%s"), __func__, *RowName.ToString());
	return false;
}

bool UDiags::GetGroup(const FName& RowName, FDiagGroup& OutGroup) const {
	if (UNLIKELY(RowName.IsNone())) return false;

	// iterate in reverse to allow to override stuff. but make add quicker.
	for (int32 i = Groups.Num()-1; i>=0; --i ) {
		UDataTable* const DT = Groups[i];

		if (UNLIKELY(!IsValid(DT))) continue;

		const FDiagGroup* const Row =
			DT->FindRow<FDiagGroup>(RowName, TEXT(""), UseWarning);
		if (UNLIKELY(!Row)) continue;

		OutGroup = *Row; // here im copying, which s-u-x. but blueprints won't take a pointer. also it's safer.
		return true;
	}

	UE_LOG(LogDiags, Verbose, TEXT("%hs Could not find sequence for row=%s"),
		__func__, *RowName.ToString());
	return false;
}

void UDiags::DiagSetEffects(const FDiag& Diag) {
	// turn old effects off
	TArray<FName> EffectsOff; // = Effects;
	TArray<FName> EffectsOn;
	EffectsOn.Reserve(Diag.Effects.Num());
	EffectsOff.Reserve(Effects.Num());
	
	for (const TPair<FName, bool>& Pair: Diag.Effects) {
		if (Pair.Value) {
			// try to add. mark as pending adding if it wasn't there
			if (LIKELY(Effects.AddUnique(Pair.Key)<0)) continue;

			EffectsOn.AddUnique(Pair.Key);
		} else {
			// if it was there, then mark as pending removal
			if (Effects.RemoveSwap(Pair.Key, EAllowShrinking::No) < 1) continue;

			EffectsOff.AddUnique(Pair.Key);
		}
	}

	Effects.Shrink(); // shrink at the end

	// turning the events off first, then on. keeping that order is a feature.
	for (const FName& N: EffectsOff)
		OnEffect.Broadcast(N, false);
	for (const FName& N: EffectsOn)
		OnEffect.Broadcast(N, true);
}

void UDiags::ShowNext() {
	if (UNLIKELY(IsShowing)) return;

	// this is important for stop to work correctly. and also in general
	IsShowing = true;
	
	if (UNLIKELY(Pending.IsEmpty())) {
		Stop();
		return;
	}

	// Do NOT get a reference here,
	// since we will remove later, it will actually return weird data.
	const FDiag Diag = Pending[0];
	Pending.RemoveAt(0);

	OnShow.Broadcast(Diag);
	
	DiagSetEffects(Diag);
}

void UDiags::Stop() {
	if (UNLIKELY(!IsShowing)) return;

	IsShowing = false;
	OnDone.Broadcast();
}

bool UDiags::CheckCondition(const FString& Expression, double& Res) const {
	Res = 0;

	if (UNLIKELY(!Eval)) {
		UE_LOG(LogDiags, Warning, TEXT("%hs Could not obtain Eval subsystem. Make sure it's enabled in config."), __func__);
		return false;
	}

	// empty expressions passes (true)
	// not unlikely because i don't know how many dialogs have an empty condition
	const bool Ok = Eval->Eval(Expression, Res, true);
	return Ok & (Res > 0);
}

void UDiags::DTCharSet(UDataTable* const DT) {
	Chars = LIKELY(IsValid(DT)) ? DT: nullptr;
}

void UDiags::DTDiagAdd(UDataTable* const DT) {
	if (UNLIKELY(!IsValid(DT))) return;
	Diags.AddUnique(DT);
}

void UDiags::DTGroupAdd(UDataTable* const DT) {
	if (UNLIKELY(!IsValid(DT))) return;
	Groups.AddUnique(DT);
}
