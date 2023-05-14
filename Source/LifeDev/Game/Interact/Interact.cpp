// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "CInteract.h"

AInteract::AInteract(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false); // ensure we don't animate on start

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	HoverMesh = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);

	Interact = CreateDefaultSubobject<UCInteract>(TEXT("Interact"));
	Interact->SetupAttachment(Mesh);
	Interact->SetComponentTickEnabled(false);
	
}

void AInteract::BeginPlay() {
	Super::BeginPlay();
	SetText();
	Interact->OnTrigger.AddUniqueDynamic(this, &AInteract::Trigger);
	Interact->OnHover.AddUniqueDynamic(this, &AInteract::Hover);
}

void AInteract::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Interact->OnTrigger.RemoveAll(this);
	Interact->OnHover.RemoveAll(this);
	HoverMesh = nullptr; // free resources to get gcd -Jero
}

void AInteract::SetText_Implementation() {}

void AInteract::SetInteractAutoBounds() {
	// Mesh->GetLocalBounds();
	// TODO how do i get the mesh size?
	Interact->Bounds = Mesh->GetPlacementExtent();
}

void AInteract::Trigger_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Actor Triggered"));
	SetText();
}

void AInteract::Hover_Implementation(bool IsOn) {
	if (!IsValid(HoverMesh)) return;
	HoverMesh->SetRenderCustomDepth(IsOn);
	// To have this working you need to enable the usage of custom stencils on the settings to
	// "Custom depth stencil pass : Enabled WITH STENCIL"
	HoverMesh->SetCustomDepthStencilValue(IsOn?255:0);
}
