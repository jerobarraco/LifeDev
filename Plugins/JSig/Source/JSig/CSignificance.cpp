// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// Probably copyright of Tom Looman too based on https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Private/Components/SSignificanceComponent.cpp
 
#include "CSignificance.h"

#include "SignificanceManager.h"

// Allows to force significance on all classes to quickly compare the performance differences as if the system was disabled.
static float GSigOverride = -1;
static FAutoConsoleVariableRef CVarSignificanceManager_SigOverride(
	TEXT("SigMan.SigOverride"),
	GSigOverride,
	TEXT("Force significance on all managed objects. -1 is default, 0-4 is hidden, lowest, medium, highest.\n"),
	ECVF_Cheat
);

UCSignificance::UCSignificance():Super() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UCSignificance::Activate(bool bReset) {
	Super::Activate(bReset);
}

void UCSignificance::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unregister(); // important or the sigmanager will leak and then crash :) (according to their docs)
	Super::EndPlay(EndPlayReason);
}

void UCSignificance::Register() {
	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;

	AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) return;
	const FName Tag = Owner->GetClass()->GetFName();

	// this is lame, but it's how it works
	auto lCalculate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) -> float
	{
		if (!IsValid(this)) return -1;
		return Calculate(ObjectInfo, Viewpoint);
	};
	
	auto lPostUpdate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
	{
		if (!IsValid(this)) return;
		PostUpdate(ObjectInfo, OldSignificance, Significance, bFinal);
	};
	
	// Register
	Man->RegisterObject(this, Tag, lCalculate, USignificanceManager::EPostSignificanceType::Sequential, lPostUpdate);
}

void UCSignificance::Unregister() {
	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;
	Man->UnregisterObject(this);
}

float UCSignificance::Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) {
	// you should pass for now.
	// TODO
	return GSigOverride;
}

void UCSignificance::PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final) {
	// pass too
	// TODO
}
