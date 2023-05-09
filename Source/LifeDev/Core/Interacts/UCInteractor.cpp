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
	FCollisionQueryParams Params;
	Params.bDebugQuery = true;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f, false, 2.f);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	
	AActor* const Actor = Hit.GetActor();
	USceneComponent* Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;
	
	if (Component && Component->IsA(UBoxComponent::StaticClass())) {
		UE_LOG(LogTemp,Log, TEXT("Is box component"));
		TArray<USceneComponent*> Parents;
		Component->GetParentComponents(Parents);
		if  (Parents.Num()>0) {
			Component = Parents[0];
		}
	}
	const bool IsInteract = IsValid(Component) && Component->IsA(UCInteract::StaticClass());
	if (IsInteract) {
		UE_LOG(LogTemp,Log, TEXT("Is interact component"));
	}

	if (!IsValid(Actor) || !Hit.Component.IsValid() || !IsInteract) {
		DoEnd();
		return;
	}
	DoStart(InteractActor);
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
	if (InteractActor) DoEnd();
	if (!IsValid(InteractActor)) return;

	InteractActor = Actor;
	OnStart.Broadcast(InteractActor);
}
#pragma optimize("", on)
