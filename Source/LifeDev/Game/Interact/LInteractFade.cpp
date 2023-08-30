#include "LInteractFade.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/CQuickMesh.h"

ALInteractFade::ALInteractFade():Super() {
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Generic/SA_GenericAttenuation.SA_GenericAttenuation"));
	SFX->AttenuationSettings = CAtt.Object;
	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Meshes.Add(Mesh);
}

void ALInteractFade::ItemRewarded_Implementation() {
	Interact->SetEnabled(false); // avoid re-rewarding due to multi clicks
	// Super::ItemRewarded_Implementation(); // not calling super since that will kill us (literally)
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteractFade::FadeDone);
	AnimFade->Play();
}

void ALInteractFade::FadeDone() {
	Destroy();
}
