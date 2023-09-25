// Copyright Jerónimo Barraco-Mármol

#include "CInteract.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCInteract, Log, Log);

UCInteract::UCInteract(): Super() {
	// these 2 seems to work ok. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
	UBoxComponent::SetComponentTickEnabled(false);
	
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	bNavigationRelevant = false;
	bCanEverAffectNavigation = false;
	// nopes because it will create collision issues.
	SetGenerateOverlapEvents(false);
	SetCanEverAffectNavigation(false);
	UBoxComponent::SetComponentTickEnabled(false);
	UBoxComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly); // it's already on the collision profile yay
	UBoxComponent::SetCollisionProfileName(CollisionProfile);
}

void UCInteract::Trigger() const {
	UE_LOG(LogCInteract, Log, TEXT("Comp Interact triggered"));
	// this is synchronous. will call AInteract(Anim, Fade) tryTrigger. and SetText at some point before this function returns
	OnTrigger.Broadcast();
}

void UCInteract::Hover(bool IsHover) const {
	// Apply the hover to the hover mesh AND all its children

	// wrapped to always trigger the delegate
	if (IsValid(HoverMesh)) {
		TArray<USceneComponent*> Children;
		HoverMesh->GetChildrenComponents(true, Children);
		Children.Add(HoverMesh);
		const int32 Num = Children.Num();
		for (int32 i = 0; i<Num; ++i) {
			UStaticMeshComponent* const Child = Cast<UStaticMeshComponent>(Children[i]);
			if (!IsValid(Child)) continue;
			
			Child->SetRenderCustomDepth(IsHover);
			// To have this working you need to enable the usage of custom stencils on the settings to
			// "Custom depth stencil pass : Enabled WITH STENCIL"
			Child->SetCustomDepthStencilValue(IsHover?255:0);
		}
	}

	OnHover.Broadcast(IsHover);
}

void UCInteract::DeInit() {
	HoverMesh = nullptr; // free resources to get gcd -Jero
}

void UCInteract::SetEnabled(bool IsEnabled) {
	SetCollisionEnabled(IsEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
