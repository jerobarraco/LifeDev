// Copyright Jerónimo Barraco-Mármol

#include "PaperI00.h"

#include "CQuickMesh.h"
#include "Components/TextRenderComponent.h"
#include "Diags/Diags.h"

APaperI00::APaperI00():Super() {
	// just for looks
	// RewardFlash = -.2f;
	// RewardItem = LDConsts::Items::Poem3;
	// TriggerDlg = "P03_Look*";

	Super::SetMobility(EComponentMobility::Static);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Mesh);
	Text->SetRelativeLocation(FVector(0.400000,-19.500000,0.600000));
	Text->SetRelativeRotation(FRotator(90,0,-90));
	// TODO add text
	Text->SetText(NSLOCTEXT("[F55AC1309BFDF396EFA8FD363F894B68]", "C2C0842F189C448F84E75A511F80204F", "Text\nnthous\nnthaeonuthoa\nnthoune\nnthnthNTH\nnth\nnth\nnth\nmoeunmao\nntheou"));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetComponentTickEnabled(false);
	Text->SetWorldSize(1);
	Text->SetYScale(1.5);
	Text->SetVertSpacingAdjust(8.5);
}
