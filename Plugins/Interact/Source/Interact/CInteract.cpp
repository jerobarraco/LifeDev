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

void UCInteract::Hover(bool IsHover) const {
	if (IsValid(HoverMesh)) {
		HoverMesh->SetRenderCustomDepth(IsHover);
		// To have this working you need to enable the usage of custom stencils on the settings to
		// "Custom depth stencil pass : Enabled WITH STENCIL"
		HoverMesh->SetCustomDepthStencilValue(IsHover?255:0);
	}
	
	OnHover.Broadcast(IsHover);
}

void UCInteract::DeInit() {
	HoverMesh = nullptr; // free resources to get gcd -Jero
}

void UCInteract::SetEnabled(bool IsEnabled) {
	SetCollisionEnabled(IsEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
