// Copyright Jerónimo Barraco-Mármol

#include "Books01.h"

#include "CQuickMesh.h"

ABooks01::ABooks01():Super(3, 0) {}


void ABooks01::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	const int32 N = Books.Num();
	if (N<2) return; // at least 2 to swap

	// cache 1st one. since we're going to overwrite
	const FTransform T0 = Books[0]->GetRelativeTransform();

	for (int32 i = 0; i<(N-1); ++i) {
		UCQuickMesh* const B = Books[i];
		const UCQuickMesh* const B2 = Books[i+1];
		if (UNLIKELY(!IsValid(B))) continue;
		if (UNLIKELY(!IsValid(B2))) continue;

		B->SetRelativeTransform(B2->GetRelativeTransform());
	}
	// note n<2 above
	Books[N-1]->SetRelativeTransform(T0);
}

void ABooks01::Constructor() {
	Super::Constructor();
	// automatic statenum
	StateNum = Books.Num();
	IsOneShot = false;
	SetAutoActivate(true);
	SetMobility(EComponentMobility::Type::Movable);
}
