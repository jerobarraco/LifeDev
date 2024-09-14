// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "JScene.h"

#include "SceneViewExtension.h"

DEFINE_LOG_CATEGORY_STATIC(LogJScene, Log, Log);

void UJScene::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	// SceneView = FSceneViewExtensions::NewExtension<>()
}

void UJScene::Deinitialize() {
	Super::Deinitialize();
}
