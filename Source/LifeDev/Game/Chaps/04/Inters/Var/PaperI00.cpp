// Copyright Jerónimo Barraco-Mármol

#include "PaperI00.h"

#include "CQuickMesh.h"
#include "Components/TextRenderComponent.h"
#include "Interact/Animator/CAnimatorFade.h"

APaperI00::APaperI00():Super() {
	// just for looks
	UseFade = true;

	Super::SetMobility(EComponentMobility::Static);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Mesh);
	Text->SetRelativeLocation(FVector(0.400000,-19.500000,0.600000));
	Text->SetRelativeRotation(FRotator(90,0,-90));
	// This text must match c4s7
	// this is ok to be here since this object is only shown by the last step (c4s9)
	Text->SetText(
		NSLOCTEXT("[F55AC1309BFDF396EFA8FD363F894B68]", "C2C0842F189C448F84E75A511F80204F",
			"Even if things around me\n don't change.\nI could change.\n"
			"I still have today.\nIt's not finished yet."));
			// "Text\nnthous\nnthaeonuthoa\nnthoune\nnthnthNTH\nnth\nnth\nnth\nmoeunmao\nntheou"));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetComponentTickEnabled(false);
	Text->SetWorldSize(1);
	Text->SetYScale(1.5);
	Text->SetVertSpacingAdjust(8.5);
}

void APaperI00::FadeUpdate(float Progress, float FadeV) {
	// FadeV 0 means showing.
	
	// unreal does not really care about alpha :'{
	Text->SetTextRenderColor(FColor::Black.WithAlpha(255.0*(1-FadeV)));
	Text->SetHiddenInGame(FadeV>.2);
	// TODO create a custom material instance and drive the parameter here
	// https://forums.unrealengine.com/t/text-render-alpha-not-working/87895/3?u=nande

	// Text->TextRenderColor.A = 255*Alpha;
	// avoid calling Text->SetTextRenderColor() which copies the value 2 times.
	// just mark it dirty.
	// Text->MarkRenderStateDirty();
}

void APaperI00::BeginPlay() {
	Super::BeginPlay();
	if (AnimFade) AnimFade->OnUpdate.AddUniqueDynamic(this, &APaperI00::FadeUpdate);
}

void APaperI00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (AnimFade) AnimFade->OnUpdate.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}
