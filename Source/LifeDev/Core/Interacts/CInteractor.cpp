// Copyright Jerónimo Barraco-Mármol

#include "CInteractor.h"

#include "CInteract.h"
#include "InteractorUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

#pragma optimize("", off)
UCInteractor::UCInteractor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
}

void UCInteractor::SetEnabled(bool Enabled) {
	SetComponentTickEnabled(Enabled);
	SetUIVisible(false);
}

void UCInteractor::TryTrigger() {
	if (!IsValid(InterComp)) return;
	InterComp->Trigger();
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	const FVector& Start = GetComponentLocation();
	const FVector& End = GetComponentRotation().Vector() * TraceLen;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, false, 1.f);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	
	USceneComponent* Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;

	// if we get a box component we need to go to the parent.
	if (Component && Component->IsA(UBoxComponent::StaticClass())) {
		TArray<USceneComponent*> Parents;
		Component->GetParentComponents(Parents);
		if  (Parents.Num() > 0) {
			Component = Parents[0];
		}
	}

	DoStart(Cast<UCInteract>(Component));
}

void UCInteractor::BeginPlay() {
	Super::BeginPlay();
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UInteractorUI>(GetOwner(), Class);
		UI->AddToViewport();
		SetUIVisible(false);
	}
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DoEnd();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::SetUIVisible(bool Visible) const {
	if (!IsValid(UI)) return;
	UI->SetVisibility(Visible? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCInteractor::DoEnd() {
	if (InterComp) {
		OnStop.Broadcast(InterComp);
		SetUIVisible(false);
	}
	InterComp = nullptr;
}

void UCInteractor::DoStart(UCInteract* Component) {
	// skip retries
	if (Component == InterComp) return;

	// notifies on changes
	if (InterComp) {
		DoEnd();
		InterComp = nullptr;
	}

	if (!IsValid(Component)) {
		return;
	}

	InterComp = Component;
	SetUIVisible(true);
	if (IsValid(UI)) {
		UI->SetPrompt(InterComp->Text);
	}
	// TODO show intercomponent text into the ui
	OnStart.Broadcast(InterComp);
}
#pragma optimize("", on)
