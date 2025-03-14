// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "BFish.h"

#include "Components/TextRenderComponent.h"

#include "CQuickMesh.h"
#include "Acts/BSleep.h"
#include "Behave/CBehave.h"
#include "Behave/Actions/BPick.h"
#include "Kismet/KismetMathLibrary.h"

#define BarScale .05

ABFish::ABFish():Super() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickEnabled(true);
	
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
	
	Behave = CreateDefaultSubobject<UCBehave>(TEXT("Behave"));
	Behave->ActionClasses.Add(UBSleep::StaticClass());
	Behave->OnState.AddUniqueDynamic(this, &ABFish::ActStateUp);
}

void ABFish::Sleep(const float DT) {
	Data.Tired = FMath::Clamp(Data.Tired - (.5*DT), 0.01, 1);
	// Behave->GetCur()->Abort(); // read notes
}

void ABFish::MoveTo(const FVector& Tgt, const float DT) {
	const FVector& Current = GetActorLocation();
	// const float Dist = FVector::DistSquared(Current, Tgt);
	// if (Dist < 1) {
	// 	// Behave->GetCur()->Stop();
	// 	return;
	// }
	const FVector& New = FMath::VInterpConstantTo(Current, Tgt, DT, MoveSpeed);
	SetActorLocation(New, false);
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Current, New));
}

void ABFish::ActStateUp(UBBase* const Act, const EBState State) {
	if (State != EBState::STARTED || !Act) {
		Doing = NAME_None;
		Text->SetText(FText::FromString("..."));
		return;
	}
	Doing = Act->ID;
	Text->SetText(FText::FromString(Doing.ToString() + "..."));
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
	MoveTgt = FMath::RandPointInBox(FBox3d(FVector(-50, -50, 0), FVector(50,50,100) ));
	// PlayPos
}

void ABFish::BeginPlay() {
	Super::BeginPlay();
}

void ABFish::Tick(const float DT) {
	Super::Tick(DT);

	Data.Tired = FMath::Clamp(Data.Tired + (.1*DT), 0.01, 1);
	S_Tired->SetRelativeScale3D(FVector(BarScale, Data.Tired, BarScale));
}

	// if (Token != UBSpace::T_Play && Doing == UBSpace::T_Play)
	// 	SetActorRotation(FRotator(0,90,0));
	//
	// Doing = Token;
	// Text->SetText(FText::FromString(Token.ToString() + "..."));
	//
	// if (Doing == UBSpace::T_Play) {
	// 	AddActorLocalRotation(FRotator(-50*DT,0,0));
	// } else if (Doing == UBSpace::T_Move) {
	// 	const FVector& Current = GetActorLocation();
	// 	const float Dist = FVector::DistSquared(Current, MoveTgt);
	// 	if (Dist < 1) {
	// 		UBSpace* const Space = Cast<UBSpace>(Behave->GetBehave(UBSpace::StaticClass()));
	// 		if (Space) Space->MoveStop();
	// 		return;
	// 	}
	// 	const FVector& New = FMath::VInterpConstantTo(Current, MoveTgt, DT, MoveSpeed);
	// 	SetActorLocation(New, false);
	// 	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Current, New));
	// 	// const FVector2D& Point = FMath::RandPointInCircle(50*DT);
	// 	// AddActorLocalOffset(FVector(Point.X, Point.Y, 0));
	// } else if (Doing == UBEmo::T_Cry) {
	// 	AddActorLocalRotation(FRotator(0,0,50*DT));
	// }

void ABFish::UpdBio(UBBase* const Behave) {
	// const UBBio* const Bio = Cast<UBBio>(Behave); // cast on every tick :( 
	// if (UNLIKELY(!Bio)) return;
	//
	// const float SCHungry = Bio->Val(UBBio::T_Hungry);
	// S_Hungry->SetRelativeScale3D(FVector(BarScale, SCHungry, BarScale));
	// const float SCTired = Bio->Val(UBBio::T_Tired);
	// S_Tired->SetRelativeScale3D(FVector(BarScale, SCTired, BarScale));
}

void ABFish::UpdEmo(UBBase* const Behave) {
	// const UBEmo* const Emo = Cast<UBEmo>(Behave); // cast on every tick :( 
	// if (UNLIKELY(!Emo)) return;
	//
	// const float SCBore = Emo->Val(UBEmo::T_Bore);
	// S_Bore->SetRelativeScale3D(FVector(BarScale, SCBore, BarScale));
}
