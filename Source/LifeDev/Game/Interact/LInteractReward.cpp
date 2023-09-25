#include "LInteractReward.h"

#include "Inventory/Inventory.h"

ALInteractReward::ALInteractReward():Super() {
	// AnimFade = CreateDefaultSubobject<UCAnimatorFade>(TEXT("AnimFade"));
	// AnimFade->Meshes.Add(Mesh);
}

void ALInteractReward::Trigger_Implementation() {
	Super::Trigger_Implementation();
	
	// reward an item if possible
	if (ItemReward.IsNone()) return;
	if (!IsValid(Inventory)) return;
	// return if we maxed out
	if (!Inventory->Mod(ItemReward, 1)) return;
	SetEnabled(false); // avoid re-rewarding due to multi clicks

	ItemRewarded();
	// if (!UseAnimFade) {
		// Faded();
        // return;
    // }
    // AnimFade->OnEnd.AddUniqueDynamic(this, &ALInteractReward::Faded);
    // AnimFade->Play();
}

// void ALInteractReward::ItemRewarded_Implementation() {}

// void ALInteractReward::Faded() {
	// AnimFade->OnEnd.RemoveAll(this);
	// if (AutoDestroy) {
		// Destroy();
	// }
// }
