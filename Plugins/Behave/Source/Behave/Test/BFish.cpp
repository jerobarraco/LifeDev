// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "BFish.h"

#include "CQuickMesh.h"
#include "Behave/CBehave.h"
#include "Behave/Behaves/Bio/BBio.h"

ABFish::ABFish():Super() {
	Behave = CreateDefaultSubobject<UCBehave>(TEXT("Behave"));
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	Body = CreateDefaultSubobject<UCQuickMesh>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Body = CreateDefaultSubobject<UCQuickMesh>(TEXT("Body"));
	Body->SetupAttachment(Root);

	Tail = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tail"));
	Tail->SetupAttachment(Body);
	Tail->SetRelativeScale3D(FVector(0.050000,0.015750,0.026250));

	
	Stats = CreateDefaultSubobject<USceneComponent>(TEXT("Stats"));
	Stats->SetupAttachment(Root);
	Stats->SetRelativeLocation(FVector(0,0,5));

	S_Hungry = CreateDefaultSubobject<UCQuickMesh>(TEXT("S_Hungry"));
	S_Hungry->SetupAttachment(Stats);
	S_Tired = CreateDefaultSubobject<UCQuickMesh>(TEXT("S_Tired"));
	S_Tired->SetupAttachment(Stats);
	S_Bore = CreateDefaultSubobject<UCQuickMesh>(TEXT("S_Bore"));
	S_Bore->SetupAttachment(Stats);
}

void ABFish::BeginPlay() {
	Super::BeginPlay();
	UBBase* const Bio = Behave->GetBehave(UBBio::StaticClass());
	if (Bio) {
		Bio->OnUpd.AddUniqueDynamic(this, &ABFish::UpdBio);
	}
}

void ABFish::UpdBio(UBBase* const Behave) {
	if (!Behave) return;
	
}
