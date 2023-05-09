// Copyright Jerónimo Barraco-Mármol

#include "UCInteract.h"

#include "Components/BoxComponent.h"

UCInteract::UCInteract(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	Collider = CreateDefaultSubobject<UBoxComponent>("Interact");
	Collider->SetupAttachment(this);
	Collider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Collider->bNavigationRelevant = false;
	Collider->SetCollisionProfileName("BlockAllDynamic");
	Collider->SetGenerateOverlapEvents(false);
}
