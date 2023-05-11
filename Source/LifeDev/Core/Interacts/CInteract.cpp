// Copyright Jerónimo Barraco-Mármol

#include "CInteract.h"

#include "Components/BoxComponent.h"

UCInteract::UCInteract(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	bNavigationRelevant = false;
	UBoxComponent::SetCollisionProfileName("BlockAllDynamic");
	SetGenerateOverlapEvents(false);
}

void UCInteract::Trigger() const {
	UE_LOG(LogTemp,Log, TEXT("Interact triggered!"));
	OnTrigger.Broadcast();
}
