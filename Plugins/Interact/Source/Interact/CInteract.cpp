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
		
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(this);
	PostProcess->bUnbound = false;
	// this only works on the constructor
	// https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assets/ReferencingAssets/
	// /Script/Engine.MaterialInstanceConstant'/Interact/MI_PostHover.MI_PostHover'
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatObj(TEXT("/Interact/MI_PostHover"));
    if (MatObj.Succeeded()) {
        PostProcess->Settings.WeightedBlendables.Array.Add({1, MatObj.Object});
    }
}

void UCInteract::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(HoverMesh)){
		PostProcess->SetActive(false);
		PostProcess->SetVisibility(false);
		PostProcess->bEnabled = false;
	}
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
