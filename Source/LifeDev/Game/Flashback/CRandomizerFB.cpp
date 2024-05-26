#include "CRandomizerFB.h" 

#include "Flashback.h"

UCRandomizerFB::UCRandomizerFB():Super() {
	DelayMin = 3;
	DelayMax = 6;
	ValueMin = -.01;
	ValueMax = .02;
	IsLooping = true;
	UCRandomizerFB::SetAutoActivate(false);
}

void UCRandomizerFB::BeginPlay() {
	Super::BeginPlay();

	Flashback = UFlashback::Instance(GetWorld());
	OnTriggerVal.AddUniqueDynamic(this, &UCRandomizerFB::TriggerFB);
}

void UCRandomizerFB::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	OnTriggerVal.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void UCRandomizerFB::TriggerFB(float Val) {
	if (!IsValid(Flashback)) return;
	Flashback->ModVal(Val);
}
