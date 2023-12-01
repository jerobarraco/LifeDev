// Copyright Jerónimo Barraco-Mármol

#include "CardLogic00.h"

#include "Diags/Diags.h"
#include "Story/Story.h"

UCardLogic00::UCardLogic00():Super() {}

void UCardLogic00::Use_Implementation() {
	Super::Use_Implementation();
	UWorld* const World = GetWorld();
	if (!World) {
		UE_LOG(LogTemp, Warning, TEXT("CardLogic00 can't find world"));
		return;
	}

	UStory* const Story = World->GetSubsystem<UStory>();
	const FName CurrentStep = Story->GetCurrent();
	UE_LOG(LogTemp, Log, TEXT("CardLogic00.Use: step=%s"), *CurrentStep.ToString());
	if (!IsValid(Diags)) {
		UE_LOG(LogTemp, Log, TEXT("CardLogic00.Use invalid diags"));
		Diags = World->GetSubsystem<UDiags>();
	}
	
	Diags->AddId("TV00_T");
}
