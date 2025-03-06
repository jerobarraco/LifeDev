// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "BFish.h"

#include "CQuickMesh.h"
#include "Behave/CBehave.h"
#include "Behave/Behaves/Bio/BBio.h"
#include "Behave/Behaves/Emo/BEmo.h"

ABFish::ABFish():Super() {
	Behave = CreateDefaultSubobject<UCBehave>(TEXT("Behave"));
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

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
	UBBase* const Emo = Behave->GetBehave(UBBio::StaticClass());
	if (Emo) {
		Emo->OnUpd.AddUniqueDynamic(this, &ABFish::UpdEmo);
	}
}

void ABFish::UpdBio(UBBase* const Behave) {
	UBBio* const Bio = Cast<UBBio>( Behave); // cast on every tick :( 
	if (!Bio) return;
	const float SCHungry = Bio->Val(UBBio::T_Hungry);
	S_Hungry->SetRelativeScale3D(FVector(.1, SCHungry, .1));
	const float SCTired = Bio->Val(UBBio::T_Tired);
	S_Tired->SetRelativeScale3D(FVector(.1, SCTired, .1));
}

void ABFish::UpdEmo(UBBase* const Behave) {
	UBEmo* const Emo = Cast<UBEmo>( Behave); // cast on every tick :( 
	if (UNLIKELY(!Emo)) return;
	const float SCBore = Emo->Val(UBEmo::T_Bore);
	S_Bore->SetRelativeScale3D(FVector(.1, SCBore, .1));
}
