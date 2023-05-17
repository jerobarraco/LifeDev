// Copyright Jerónimo Barraco-Mármol

#include "Interact.h"

#include "CInteract.h"

AInteract::AInteract(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer) {
	// super important or it will NOT work
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false); // ensure we don't animate on start

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
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
	Interact->HoverMesh = Mesh;

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(Mesh);
	// TODO fix this. 
	static ConstructorHelpers::FObjectFinder<UObject> MatObj(TEXT("/Game/Interact/MI_PostHover"));
    if (MatObj.Succeeded()) {
        PostProcess->Settings.WeightedBlendables.Array.Add({1, MatObj.Object});
    } 
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
	Interact->DeInit();
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

void AInteract::Hover_Implementation(bool IsOn) {}
