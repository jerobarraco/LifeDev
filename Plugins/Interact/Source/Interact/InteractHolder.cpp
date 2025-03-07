// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "InteractHolder.h"


AInteractHolder::AInteractHolder():Super() {
	Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	// so that it moves along with the animation
	Item->SetupAttachment(IRoot);
}

// TODO figure out why this does not work.
// void AInteractHolder::PostInitProperties() {
	// Super::PostInitProperties();
	// Item->SetChildActorClass(ItemClass);
	// Item->SetRelativeTransform(ItemTrans, false, nullptr, ETeleportType::TeleportPhysics);
// }

