// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "JScene.h"

#include "JSceneView.h"
#include "SceneViewExtension.h"

DEFINE_LOG_CATEGORY_STATIC(LogJScene, Log, Log);

void UJScene::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	SceneView = FSceneViewExtensions::NewExtension<FJSceneView>(this);
}

void UJScene::Deinitialize() {
	if (SceneView) {
		SceneView->IsActiveThisFrameFunctions.Empty();
		FSceneViewExtensionIsActiveFunctor IsActiveFunctor;
		IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension,
			const FSceneViewExtensionContext& Context){
			return TOptional<bool>(false);
		};
		SceneView->IsActiveThisFrameFunctions.Add(IsActiveFunctor);

		SceneView->Invalidate();
		SceneView.Reset(); // resets the shared ptr
	}
	SceneView = nullptr;
	
	Super::Deinitialize();
}
