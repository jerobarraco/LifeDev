// Copyright Jerónimo Barraco-Mármol

#include "UCInteractor.h"

#include "UCInteract.h"
#include "Components/BoxComponent.h"

#pragma optimize("", off)
UCInteractor::UCInteractor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	const FVector& Start = GetComponentLocation();
	const FVector& End = GetComponentRotation().Vector() * TraceLen;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.f, false, .5f);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	
	AActor* Actor = nullptr;
	USceneComponent* Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;

	// if we get a box component we need to go to the parent.
	if (Component && Component->IsA(UBoxComponent::StaticClass())) {
		TArray<USceneComponent*> Parents;
		Component->GetParentComponents(Parents);
		if  (Parents.Num() > 0) {
			Component = Parents[0];
		}
	}

	const bool IsInteract = IsValid(Component) && Component->IsA(UCInteract::StaticClass());
	if (IsInteract) {
		Actor = Hit.GetActor();
	}

	DoStart(Actor);
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DoEnd();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	if (InteractActor) {
		OnEnd.Broadcast(InteractActor);
	}
	InteractActor = nullptr;
}

void UCInteractor::DoStart(AActor* Actor) {
	// skip retries
	if (Actor == InteractActor) return;

	// notifies on changes
	if (InteractActor) {
		DoEnd();
		InteractActor = nullptr;
	}

	if (!IsValid(Actor)) {
		return;
	}

	InteractActor = Actor;
	OnStart.Broadcast(InteractActor);
}
#pragma optimize("", on)
