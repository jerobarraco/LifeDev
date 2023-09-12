// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
 
#include "CSignificance.h"

// loosely based on https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Private/Components/SSignificanceComponent.cpp
UCSignificance::UCSignificance():Super() {
	PrimaryComponentTick.bCanEverTick = false;

}

void UCSignificance::Activate(bool bReset) {
	Super::Activate(bReset);
	// if (USignificanceManager* SignificanceManager = USignificanceManager::Get(GetWorld()))
	// {
	// 	auto SignificanceFunc = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) -> float
	// 	{
	// 		return CalcSignificance(ObjectInfo, Viewpoint);
	// 	};
	// 	
	// 	auto PostSignificanceFunc = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal)
	// 	{
	// 		PostSignificanceUpdate(ObjectInfo, OldSignificance, Significance, bFinal);
	// 	};
	//
	// 	// Register
	// 	FName Tag = GetOwner()->GetClass()->GetFName();
	// 	SignificanceManager->RegisterObject(this, Tag, SignificanceFunc, USignificanceManager::EPostSignificanceType::Sequential, PostSignificanceFunc);
	// }
}
