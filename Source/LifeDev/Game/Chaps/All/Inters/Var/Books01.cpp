// Copyright Jerónimo Barraco-Mármol

#include "Books01.h"

#include "CQuickMesh.h"

ABooks01::ABooks01():Super(3, 0) {
	ConstructB01();
}

ABooks01::ABooks01(const int32 Seed):Super(3, Seed) {
	ConstructB01();
}

void ABooks01::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	const int32 N = Books.Num();
	// at least 2 to swap. and need the book trans
	if (N<2 || BookTrans.Num() < N) return;

	// cache 1st one. since we're going to overwrite
	for (int32 i = 0; i<N; ++i) {
		UCQuickMesh* const B = Books[i];
		if (UNLIKELY(!IsValid(B))) continue;

		B->SetRelativeTransform(BookTrans[(i+State)%N]);
	}
	// TODO animation. Can't use fade since it will force the same material on all
}

void ABooks01::ConstructB01() {
	for (const UCQuickMesh* const B: Books) {
		if (UNLIKELY(!B)) continue;
		BookTrans.Add(B->GetRelativeTransform());
	}

	// for some reason this can't be done in the constructor. though it'd really help
	// TODO try moving some stuff and see what breaks
	// automatic statenum
	StateNum = Books.Num();
	IsOneShot = false;
	Texts = {
		NSLOCTEXT("Books01", "Text.0", "Rearrange"),
		NSLOCTEXT("Books01", "Text.1", "Rearrange again"),
		NSLOCTEXT("Books01", "Text.2", "Rearrange once more"),
	};
	SetAutoActivate(true);
	SetMobility(EComponentMobility::Type::Movable);
}
