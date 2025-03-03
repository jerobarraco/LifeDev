// Copyright Jerónimo Barraco-Mármol

#include "Books01.h"

#include "CQuickMesh.h"

ABooks01::ABooks01():Super(3, 0) {
	for (const UCQuickMesh* const B: Books) {
		if (UNLIKELY(!B)) continue;
		BookTrans.Add(B->GetRelativeTransform());
	}

	// automatic statenum
	StateNum = Books.Num();
	IsOneShot = false;
	SetAutoActivate(true);
	SetMobility(EComponentMobility::Type::Movable);
}

void ABooks01::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	const int32 N = Books.Num();
	if (N<2) return; // at least 2 to swap

	// cache 1st one. since we're going to overwrite
	for (int32 i = 0; i<N; ++i) {
		UCQuickMesh* const B = Books[i];
		if (UNLIKELY(!IsValid(B))) continue;

		B->SetRelativeTransform(BookTrans[(i+State)%N]);
	}
	// TODO animation. Can't use fade since it will force the same material on all
}
