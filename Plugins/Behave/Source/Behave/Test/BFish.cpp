// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "BFish.h"

#include "CQuickMesh.h"
#include "Behave/CBehave.h"

ABFish::ABFish():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Body = CreateDefaultSubobject<UCQuickMesh>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Tail = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tail"));
	Tail->SetupAttachment(Body);
	Behave = CreateDefaultSubobject<UCBehave>(TEXT("Behave"));
}
