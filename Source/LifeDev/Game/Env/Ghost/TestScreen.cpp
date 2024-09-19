// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "TestScreen.h"

#include "JUtils/JMiscUtils.h"

void ATestScreen::BeginPlay() {
	Super::BeginPlay();
	FScreenshotRequest::OnScreenshotCaptured().AddUObject(this, &ATestScreen::Captured);
	FScreenshotRequest::RequestScreenshot(false);
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
}

void ATestScreen::Tick(float DeltaSeconds) {
	// Super::Tick(DeltaSeconds);
	// UGameViewportClient* ViewportClient = UJMiscUtils::GetAnyGameViewportClient();
	// static TArray<FColor> Bmp;
	// View->UnscaledViewRect;
	// GetViewportScreenShot(ViewportClient, Bmp, ViewportClient->)
	// https://github.com:EpicGames/UnrealEngine/blob/5.4/Engine/Plugins/AI/MLAdapter/Source/MLAdapter/Private/Sensors/MLAdapterSensor_Camera.cpp#L159-L159
	// GetHighResScreenshotConfig().SetResolution(width, height, 200.f);
	// GetHighResScreenshotConfig().FilenameOverride = fileFullpath = FPaths::ProjectDir() / TEXT(“Snapshots”) / fileName;
	// FScreenshotRequest::RequestScreenshot(false);
	// https://forums.unrealengine.com/t/how-to-take-high-res-screenshots-shipping-builds/441246/5?u=nande	
	FScreenshotRequest::RequestScreenshot(false);
}

void ATestScreen::Captured(int32 Width, int32 Height, const TArray<FColor>& Colors) {
	UE_LOG(LogTemp, Log, TEXT("...%hs %i %i"), __func__, Width, Height);
	
}
