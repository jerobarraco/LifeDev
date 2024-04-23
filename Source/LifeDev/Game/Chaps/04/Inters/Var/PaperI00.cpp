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
	Text->SetRelativeLocation(FVector(0.398611,-19.676087,0.669464));
	Text->SetRelativeRotation(FRotator(90,693.434949,603.434950));
	Text->SetText(NSLOCTEXT("[F55AC1309BFDF396EFA8FD363F894B68]", "C2C0842F189C448F84E75A511F80204F", "Text\nnthous\nnthaeonuthoa\nnthoune\nnthnthNTH\nnth\nnth\nnth\nmoeunmao\nntheou"));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextTop);
	Text->SetTextRenderColor(FColor::Black);
	Text->SetWorldSize(2);
	Text->SetComponentTickEnabled(false);
	
	// TODO add text
}

void APaperI00::BeginPlay() {
	Super::BeginPlay();
	// Diags->GetDiag()
}
