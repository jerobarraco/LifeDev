// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "JSceneView.h"

#include "DynamicResolutionState.h"
#include "FXRenderingUtils.h"
#include "JScene.h"
#include "SceneViewExtension.h"
#include "PostProcess/PostProcessInputs.h"

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

/*
void FJSceneView::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View,
	const FPostProcessingInputs& Inputs) {
	if (!IsValid(Subsystem)) return;
	
	const FSceneViewFamily& ViewFamily = *View.Family;

	// We need to make sure to take Windows and Scene scale into account.
	float ScreenPercentage = ViewFamily.SecondaryViewFraction;
	const ISceneViewFamilyScreenPercentage* ScrPerInt = ViewFamily.GetScreenPercentageInterface();
	if (ScrPerInt) {
		DynamicRenderScaling::TMap<float> UpperBounds = ScrPerInt->GetResolutionFractionsUpperBound();
		ScreenPercentage *= UpperBounds[GDynamicPrimaryResolutionFraction];
	}

	// get the viewrect
	const FIntRect PrimaryViewRect = UE::FXRenderingUtils::GetRawViewRectUnsafe(View);
	FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, PrimaryViewRect);

	if (!SceneColor.IsValid()) return;

	FSceneViewExtensionBase::PrePostProcessPass_RenderThread(GraphBuilder, View, Inputs);
	
	// Reusing the same output description for our back buffer as SceneColor
	FRDGTextureDesc OutputDesc = SceneColor.Texture->Desc;
	OutputDesc.Format = PF_FloatRGBA;
	constexpr FLinearColor ClearColor(0., 0.5, 0.5, 0.);
	OutputDesc.ClearValue = FClearValueBinding(ClearColor);


	FRDGTexture* const BackBufferRTT = GraphBuilder.CreateTexture(OutputDesc, TEXT("BackBufferRenderTargetTexture"));
	FScreenPassRenderTarget BackBufferRT = FScreenPassRenderTarget(BackBufferRTT, SceneColor.ViewRect, ERenderTargetLoadAction::EClear);
	FScreenPassRenderTarget SceneColorRT(SceneColor, ERenderTargetLoadAction::ELoad);

}
*/

void FJSceneView::Invalidate() {
	Subsystem = nullptr;
}
