// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class UJScene;

class JSCENE_API FJSceneView : public FSceneViewExtensionBase {
public:
	FJSceneView(const FAutoRegister& AutoRegister, UJScene* InSubsystem);

	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;

	// Called when owning subsystem needs to release this extension.
	void Invalidate();
protected:
	UJScene* Subsystem = nullptr;
};
