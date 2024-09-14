// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "JSceneView.h"

#include "SceneViewExtension.h"

DEFINE_LOG_CATEGORY_STATIC(LogJSceneView, Log, Log);

FJSceneView::FJSceneView(const FAutoRegister& AutoRegister, UJScene* InSubsystem) :
	FSceneViewExtensionBase(AutoRegister) {
	Subsystem = InSubsystem;
}

void FJSceneView::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) {
	
}

void FJSceneView::SetupViewFamily(FSceneViewFamily& InViewFamily) {
	
}

void FJSceneView::BeginRenderViewFamily(FSceneViewFamily& InViewFamily) {}
