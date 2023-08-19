#include "LInteractFade.h"

#include "Components/AudioComponent.h"
#include "Interact/Animator/CAnimatorFade.h"

ALInteractFade::ALInteractFade():Super() {
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/SA_GenericAttenuation.SA_GenericAttenuation"));
	SFX->AttenuationSettings = CAtt.Object;
	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Meshes.Add(Mesh);
}

void ALInteractFade::FadeDone() {
	Destroy();
}

void ALInteractFade::ItemRewarded_Implementation() {
	// Super::ItemRewarded_Implementation(); // not calling super since that will kill us (literally)
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteractFade::FadeDone);
	AnimFade->Play();
}