// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "BFish.h"

#include "CQuickMesh.h"
#include "Behave/CBehave.h"
#include "Behave/Behaves/Bio/BBio.h"
#include "Behave/Behaves/Emo/BEmo.h"
#include "Behave/Behaves/Space/BSpace.h"
#include "Components/TextRenderComponent.h"

#define BarScale .05

ABFish::ABFish():Super() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickEnabled(true);

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
	S_Bore->SetRelativeScale3D(FVector(BarScale, BarScale, BarScale));
	S_Hungry->SetRelativeScale3D(FVector(BarScale, BarScale, BarScale));
	S_Tired->SetRelativeScale3D(FVector(BarScale, BarScale, BarScale));

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(Root);
	Text->SetRelativeLocation(FVector(0,0,15));
}

static const FVector FoodPos(50, 50, 50);
static const FVector SleepPos(10, 10, 10);
static const FVector PlayPos(10, -50, 30);

bool ABFish::CanEat() {
	return FVector::DistSquared(FoodPos, GetActorLocation()) < 10; 
}

void ABFish::MoveToFood() {
	MoveTgt = FoodPos;
}

void ABFish::MoveToSleep() {
	MoveTgt = SleepPos;
}

void ABFish::MoveToPlay() {
	MoveTgt = PlayPos;
}

void ABFish::BeginPlay() {
	Super::BeginPlay();
	UBBase* const Bio = Behave->GetBehave(UBBio::StaticClass());
	if (Bio) {
		Bio->OnUpd.AddUniqueDynamic(this, &ABFish::UpdBio);
	}
	UBBase* const Emo = Behave->GetBehave(UBEmo::StaticClass());
	if (Emo) {
		Emo->OnUpd.AddUniqueDynamic(this, &ABFish::UpdEmo);
	}

	UBSpace* const Space = Cast<UBSpace>(Behave->GetBehave(UBSpace::StaticClass()));
	if (Space) {
		Space->OnFoodClose.BindDynamic(this, &ABFish::CanEat);
		Space->OnMoveToFood.BindDynamic(this, &ABFish::MoveToFood);
		Space->OnMoveToSleep.BindDynamic(this, &ABFish::MoveToSleep);
		Space->OnMoveToPlay.BindDynamic(this, &ABFish::MoveToPlay);
		// Space->OnMoveToFood.BindDynamic(this, &ABFish::MoveToFood);
		// Space->OnUpd.AddUniqueDynamic(this, &ABFish::UpdEmo);
	}

	Behave->OnDo.AddUniqueDynamic(this, &ABFish::Do);
}

void ABFish::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void ABFish::Do(const FName& Token, const float DT) {
	if (Token != UBSpace::T_Play && Doing == UBSpace::T_Play)
		SetActorRotation(FRotator(0,90,0));

	Doing = Token;
	Text->SetText(FText::FromString(Token.ToString() + "..."));

	if (Doing == UBSpace::T_Play) {
		AddActorLocalRotation(FRotator(-50*DT,0,0));
	} else if (Doing == UBSpace::T_Move) {
		const FVector& Current = GetActorLocation();
		const float Dist = FVector::DistSquared(Current, MoveTgt);
		if (Dist < 1) {
			UBSpace* const Space = Cast<UBSpace>(Behave->GetBehave(UBSpace::StaticClass()));
			if (Space) Space->MoveStop();
			return;
		}
		const FVector& New = FMath::VInterpConstantTo(Current, MoveTgt, DT, 10);
		SetActorLocation(New, false);
		// const FVector2D& Point = FMath::RandPointInCircle(50*DT);
		// AddActorLocalOffset(FVector(Point.X, Point.Y, 0));
	}
}

void ABFish::UpdBio(UBBase* const Behave) {
	const UBBio* const Bio = Cast<UBBio>(Behave); // cast on every tick :( 
	if (UNLIKELY(!Bio)) return;

	const float SCHungry = Bio->Val(UBBio::T_Hungry);
	S_Hungry->SetRelativeScale3D(FVector(BarScale, SCHungry, BarScale));
	const float SCTired = Bio->Val(UBBio::T_Tired);
	S_Tired->SetRelativeScale3D(FVector(BarScale, SCTired, BarScale));
}

void ABFish::UpdEmo(UBBase* const Behave) {
	const UBEmo* const Emo = Cast<UBEmo>(Behave); // cast on every tick :( 
	if (UNLIKELY(!Emo)) return;

	const float SCBore = Emo->Val(UBEmo::T_Bore);
	S_Bore->SetRelativeScale3D(FVector(BarScale, SCBore, BarScale));
}
