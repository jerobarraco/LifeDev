#include "LInteractFade.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ALInteractFade::ALInteractFade():Super() {
	// TODO (not sure) move this to LInteract and Make LNPC01I00 use this fader
	// need an option to enable or not the fader (?) only do so on item reward. not all item rewards will fade. all automatic item rewards will destroy.
	AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	AnimFade->Meshes.Add(Mesh);
}

void ALInteractFade::ItemRewarded_Implementation() {
	// Super::ItemRewarded_Implementation(); // not calling super since that will kill us (literally)
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteractFade::FadeDone);
	AnimFade->Play();
}

void ALInteractFade::FadeDone() {
	Destroy();
}
