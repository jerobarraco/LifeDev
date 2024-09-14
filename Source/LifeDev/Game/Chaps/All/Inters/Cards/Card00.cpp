// Copyright Jerónimo Barraco-Mármol

#include "Card00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard00::ACard00():Super() {
	RewardItem = LDConsts::Items::Card0;
	TriggerDlg = "C0_Pick";
	UE_LOG(LogTemp, Log, TEXT("TTT %hs"), __func__);
}

void ACard00::PostInitProperties() {
	Super::PostInitProperties();
	UE_LOG(LogTemp, Log, TEXT("TTT %hs"), __func__);
}

void ACard00::PostInitializeComponents() {
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Log, TEXT("TTT %hs"), __func__);
}

void ACard00::PostActorCreated() {
	Super::PostActorCreated();
	UE_LOG(LogTemp, Log, TEXT("TTT %hs"), __func__);
}

void ACard00::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TTT %hs"), __func__);
}
