#include "CRandomizerFB.h" 

#include "Flashback.h"

UCRandomizerFB::UCRandomizerFB():Super() {
	DelayMin = 2.5;
	DelayMax = 5;
	ValueMin = -.02;
	ValueMax = .05;
	IsLooping = true;
}

void UCRandomizerFB::BeginPlay() {
	Super::BeginPlay();
	OnTriggerVal.AddUniqueDynamic(this, &UCRandomizerFB::TriggerFB);
}

void UCRandomizerFB::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	OnTriggerVal.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void UCRandomizerFB::TriggerFB(float Val) {
	GetWorld()->GetSubsystem<UFlashback>()->IncVal(Val);
}
