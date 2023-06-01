// Copyright Jerónimo Barraco-Mármol

#include "CInteractAnim.h"

#include "CAnimatorTrans.h"
#include "CInteract.h"

// This doesn't friking work because ue doesn't really support nested components or nobody knows how to.
// https://forums.unrealengine.com/t/how-do-you-make-a-component-with-nested-components/375589/4
UCInteractAnim::UCInteractAnim(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IMesh"));
	// Mesh->SetupAttachment(Root);
	Mesh->PrimaryComponentTick.bStartWithTickEnabled = false;
	Mesh->SetComponentTickEnabled(false);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Mesh->SetCanEverAffectNavigation(false);
	
	Interact = CreateDefaultSubobject<UCInteract>(TEXT("IInteract"));
	Interact->SetupAttachment(Mesh);
	// Interact->SetComponentTickEnabled(false);
	Interact->HoverMesh = Mesh;
	
	Animator = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("IAnimator"));
	// Animator->RegisterComponent();
	Animator->AnimRoot = this;
}

void UCInteractAnim::PostInitProperties() {
	Super::PostInitProperties();
}

void UCInteractAnim::PostCDOContruct() {
	Super::PostCDOContruct();
	Mesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	Interact->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void UCInteractAnim::BeginPlay() {
	Super::BeginPlay();
	Interact->OnTrigger.AddUniqueDynamic(this, &UCInteractAnim::Trigger);
}

void UCInteractAnim::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

void UCInteractAnim::Trigger() {
	UE_LOG(LogTemp,Log, TEXT("Interact triggered!"));
	if (Animator->GetIsAnimating()) return;
	if (Locked) return;

	SetText();
	Animator->Play(IsOpen);

	// change the flag after we start rotating. now the change has happened.
	IsOpen = !IsOpen;
	UE_LOG(LogTemp, Log, TEXT("InteractAnim changed open=%i"), IsOpen ? 0:1);
}

void UCInteractAnim::Hover(bool IsHover) const {
}

void UCInteractAnim::DeInit() {
	Interact->OnTrigger.RemoveAll(this);
	Interact->DeInit();
	Animator->DeInit();
}

void UCInteractAnim::SetEnabled(bool IsEnabled) {
}

void UCInteractAnim::SetText() {}
