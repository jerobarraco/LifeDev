// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Modules/ModuleInterface.h"

// You need to set the InteractChannel on CInteract.cpp
// you need to go to the project settings and add a custom trace channel and custom collision profile
// Project settings-> Collisions
// the trace channel needs to be called "Interact" and set default to "Ignore"
// the object type should be world dynamic
// the physics type should be "Query only" (but has to be query)

// the collision profile needs to be called Interact and set Interact, Camera, Visible channels to block, the rest to ignore

// to get the collision channel read https://gamedev.stackexchange.com/a/178812/44866
// at runtime set it on CInteractor::SetCollisionChannel
// https://www.youtube.com/watch?v=QQvHZLB4CTU

class INTERACT_API FInteractModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
