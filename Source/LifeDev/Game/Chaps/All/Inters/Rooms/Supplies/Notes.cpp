// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Notes.h"

#include "CQuickMesh.h"
#include "Components/TextRenderComponent.h"

ANotes::ANotes() {
	
	
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Mesh);
	Text->SetRelativeLocation(FVector(0.400000,-19.500000,0.600000));
	Text->SetRelativeRotation(FRotator(90,0,-90));
	// Text->SetText(NSLOCTEXT("", "", ""));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetComponentTickEnabled(false);
	Text->SetWorldSize(1);
	Text->SetYScale(1.5);
	Text->SetVertSpacingAdjust(8.5);
}
