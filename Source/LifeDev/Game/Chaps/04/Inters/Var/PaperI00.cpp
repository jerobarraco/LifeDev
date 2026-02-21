// Copyright Jerónimo Barraco-Mármol

#include "PaperI00.h"

#include "Components/TextRenderComponent.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

APaperI00::APaperI00():Super() {
	// just for looks
	UseFade = true;
	UseAutoActivate = false; // we got one that can't be interacted. override parent.
	SetActorHiddenInGame(true);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Mesh);
	Text->SetRelativeLocation(FVector(0.400000,-19.500000,0.600000));
	Text->SetRelativeRotation(FRotator(90,0,-90));
	// This text must match c4s7
	// this is ok to be here since this object is only shown by the last step (c4s9)
	Text->SetText(
		NSLOCTEXT("PaperI00", "Text",
			"Even if things around me\n don't change.\nI could change.\n"
			"I still have today.\nIt's not over yet."));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetComponentTickEnabled(false);
	Text->SetWorldSize(1);
	Text->SetYScale(1.5);
	Text->SetVertSpacingAdjust(8.5);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CText(TEXT("/JUtils/Mats/Mask/FadeText_MD"));
	Text->SetMaterial(0, CText.Object); // call SetMaterial to use Create... later.
	
	Super::SetMobility(EComponentMobility::Static);
}

void APaperI00::FadeUpdate(const float Progress, const float FadeV) {
	// FadeV 0 means showing.

	// unreal does not really care about alpha :'{
	// Text->SetTextRenderColor(FColor::Black.WithAlpha(255.0*(1-FadeV)));
	// Text->SetHiddenInGame(FadeV>.2);
	
	// https://forums.unrealengine.com/t/text-render-alpha-not-working/87895/3?u=nande
	if (LIKELY(TextMID)) TextMID->SetScalarParameterValue("Fade", 1-FadeV);

	// Text->TextRenderColor.A = 255*Alpha;
	// avoid calling Text->SetTextRenderColor() which copies the value 2 times.
	// just mark it dirty.
	// Text->MarkRenderStateDirty();
}

void APaperI00::BeginPlay() {
	Super::BeginPlay();
	if (LIKELY(AnimFade))
		AnimFade->OnUpdate.AddUniqueDynamic(this, &APaperI00::FadeUpdate);

	// create the mid. nullptr means to use the material set with SetMaterial on constructor.
	TextMID = Text->CreateDynamicMaterialInstance(0, nullptr);
	Text->SetTextMaterial(TextMID); // the above calls SetMaterial, but not SetTextMaterial
}

void APaperI00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(AnimFade))
		AnimFade->OnUpdate.RemoveAll(this);
	TextMID = nullptr;
	Super::EndPlay(EndPlayReason);
}
