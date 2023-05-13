// Copyright Jerónimo Barraco-Mármol

#include "CInteract.h"

#include "Components/BoxComponent.h"

UCInteract::UCInteract(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	bNavigationRelevant = false;
	UBoxComponent::SetCollisionProfileName("BlockAllDynamic");
	UBoxComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetGenerateOverlapEvents(false);
	UBoxComponent::SetComponentTickEnabled(false);
}

void UCInteract::Trigger() const {
	UE_LOG(LogTemp,Log, TEXT("Interact triggered!"));
	OnTrigger.Broadcast();
}

void UCInteract::SetEnabled(bool IsEnabled) {
	SetCollisionEnabled(IsEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
