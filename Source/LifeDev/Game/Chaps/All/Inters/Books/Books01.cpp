// Copyright Jerónimo Barraco-Mármol

#include "Books01.h"

#include "CQuickMesh.h"
#include "Interact/Animator/Anim.h"

ABooks01::ABooks01() { // Super(3, 0)
	BookCount = 3;
	ConstructB01();
}

void ABooks01::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	const int32 N = Books.Num();
	// at least 2 to swap. and need the book trans
	if (UNLIKELY(N<2 || BookTrans.Num() < N)) return;

	UAnim* const AnimSub = UAnim::Instance(this);
	
	// cache 1st one. since we're going to overwrite
	for (int32 i = 0; i<N; ++i) {
		UCQuickMesh* const B = Books[i];
		if (UNLIKELY(!IsValid(B))) continue;

		B->SetRelativeTransform(BookTrans[(i+State)%N]);
		// a very basic animation so it does not look awful
		// i can't use a CAnimatorMix because each book has its own material :')
		if (LIKELY(AnimSub)) {
			UMaterialInstanceDynamic* const Mat = Cast<UMaterialInstanceDynamic>(B->GetMaterial(0));
			if (LIKELY(Mat)) {
				FAParams P{.Name = "Fade", .Duration = 0};
				AnimSub->DynFloatFade(Mat, P, 1);// set now, no curve.
				P.Duration = .5;
				P.Curve = FadeCurve;
				AnimSub->DynFloatFade(Mat, P, 0);
			}
		}
	}
	// TODO improve animation
}

void ABooks01::BeginPlay() {
	Super::BeginPlay();
	
	for (UCQuickMesh* const B: Books) {
		if (UNLIKELY(!B)) continue;
		B->CreateDynamicMaterialInstance(0);
	}
}

void ABooks01::ConstructB01() {
	IsOneShot = false;
	UseAutoActivate = true;

	for (const UCQuickMesh* const B: Books) {
		if (UNLIKELY(!B)) continue;
		BookTrans.Add(B->GetRelativeTransform());
	}
	StateNum = Books.Num(); // automatic statenum
	Texts = {
		NSLOCTEXT("Books01", "Text.0", "Rearrange"),
		NSLOCTEXT("Books01", "Text.1", "Rearrange again"),
		NSLOCTEXT("Books01", "Text.2", "Rearrange once more"),
	};
	SetMobility(EComponentMobility::Type::Movable);

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/InCubic_C.InCubic_C"));
	FadeCurve = CCurve.Object;
}
