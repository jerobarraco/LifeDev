// Copyright Jerónimo Barraco-Mármol

#include "CInteractor.h"

#include "Components/ArrowComponent.h"

#include "CInteract.h"

#pragma optimize("", off)
UCInteractor::UCInteractor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);

	// TODO make the arrow parent correctly
	IArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("IArrow"));
	IArrow->SetupAttachment(this);
	IArrow->SetComponentTickEnabled(false);
	IArrow->SetArrowLength(TraceLen);
}

void UCInteractor::SetEnabled(bool Enabled) {
	SetComponentTickEnabled(Enabled);
	DoEnd(); // force clearing currently selected
}

void UCInteractor::TryTrigger() {
	if (!IsValid(InterComp)) return;
	InterComp->Trigger();
}

void UCInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult Hit;
	const FVector& Start = GetComponentLocation();
	const FVector& Direction = GetComponentRotation().Vector() * TraceLen;
	const FVector& End = Start + Direction;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, false, 1.f);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
	
	USceneComponent* Component = Hit.Component.IsValid() ? Hit.Component.Get() : nullptr;
	UCInteract* const Interact = Cast<UCInteract>(Component);
	DoStart(Interact);
}

void UCInteractor::BeginPlay() {
	Super::BeginPlay();
}

void UCInteractor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DoEnd();
	Super::EndPlay(EndPlayReason);
}

void UCInteractor::DoEnd() {
	if (!InterComp) return;
	if (IsValid(InterComp)) {
		InterComp->Hover(false);
	}
	OnToggle.Broadcast(false, InterComp);
	OnStop.Broadcast(InterComp);

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
	InterComp->Hover(true);

	OnToggle.Broadcast(true, InterComp);
	OnStart.Broadcast(InterComp);
}
#pragma optimize("", on)
