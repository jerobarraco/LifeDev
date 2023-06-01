// Copyright Jerónimo Barraco-Mármol

#include "CInteractPart.h"

#include "CAnimatorTrans.h"
#include "CInteract.h"
#include "Components/BoxComponent.h"

UCInteractPart::UCInteractPart(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IMesh"));
	// Mesh->SetupAttachment(Root);
	Mesh1->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh1->SetComponentTickEnabled(false);
	Mesh1->SetGenerateOverlapEvents(false);
	Mesh1->SetCollisionProfileName("NoCollision");
	Mesh1->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh1->SetCanEverAffectNavigation(false);
	
	Interact1 = CreateDefaultSubobject<UCInteract>(TEXT("IInteract"));
	Interact1->SetupAttachment(Mesh1);
	// Interact->SetComponentTickEnabled(false);
	Interact1->HoverMesh = Mesh1;
	
	Animator1 = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("IAnimator"));
	// Animator->RegisterComponent();
	Animator1->AnimRoot = this;
}

void UCInteractPart::PostInitProperties() {
	Super::PostInitProperties();
}

void UCInteractPart::PostCDOContruct() {
	Super::PostCDOContruct();
	Mesh1->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Interact1->AttachToComponent(Mesh1, FAttachmentTransformRules::KeepRelativeTransform);
}

void UCInteractPart::BeginPlay() {
	Super::BeginPlay();
	Interact1->OnTrigger.AddUniqueDynamic(this, &UCInteractPart::Trigger);
}

void UCInteractPart::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

void UCInteractPart::Trigger() {
	UE_LOG(LogTemp,Log, TEXT("Interact triggered!"));
	if (Animator1->GetIsAnimating()) return;
	if (Locked) return;

	SetText();
	Animator1->Play(IsOpen);

	// change the flag after we start rotating. now the change has happened.
	IsOpen = !IsOpen;
	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}

void UCInteractPart::Hover(bool IsHover) const {
}

void UCInteractPart::DeInit() {
	Interact1->OnTrigger.RemoveAll(this);
	Interact1->DeInit();
	Animator1->DeInit();
}

void UCInteractPart::SetEnabled(bool IsEnabled) {
}

void UCInteractPart::SetText() {}
