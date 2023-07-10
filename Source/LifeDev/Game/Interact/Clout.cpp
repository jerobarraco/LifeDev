// Copyright Jerónimo Barraco-Mármol

#include "Clout.h"

#include "Interact/Animator/CAnimatorMix.h"

AClout::AClout():Super() {
	AnimEnabled = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	// ItemReward = TEXT("C1C"); // chapter 1 clout 0
	// Mesh->SetRelativeLocation(FVector(-17.50,17.50,-0.));
	// Interact->SetRelativeLocation(FVector(17.50,-17.50,2.50));
	// Interact->SetBoxExtent(FVector(18.,18.,3.));
	Texts[0] = FText(NSLOCTEXT("Clout", "PickUp", "Pick up"));
	// TriggerDlg = FName("C");
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
}
