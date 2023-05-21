// Copyright Jerónimo Barraco-Mármol

#include "InteractHolder.h"


AInteractHolder::AInteractHolder():Super() {
	Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	// so that it moves along with the animation
	Item->SetupAttachment(IRoot);
}

// TODO figure out why this does not works.
// void AInteractHolder::PostInitProperties() {
	// Super::PostInitProperties();
	// Item->SetChildActorClass(ItemClass);
	// Item->SetRelativeTransform(ItemTrans, false, nullptr, ETeleportType::TeleportPhysics);
// }

