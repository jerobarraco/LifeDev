// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"

// #include "DefaultCameraShakeBase.h"
#include "Camera/CameraComponent.h"

#include "Diags/Diags.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;

	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CShake(TEXT("/Game/LifeDev/Game/Env/CamShake_B")); // camshake doesn't work with the ".CamShake_B" ending 
	ShakeClass = CShake.Succeeded() ? CShake.Class.Get() : ShakeClass;
	// UDefaultCameraShakeBase::StaticClass(); // disappeared from ue5.4 without warning

	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);
	GhostPos = FVector(200,-4,-75);
	UseGhosts = true;
	ItemsRem = { "T00"}; // use up the item
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	
	FB->SetMax(1); // reset to 1 since we will change it several times here
	FB->SetVal(.75); // was already clamped to .7 on c1s0, so it can't be bigger
	FbDiagMod = (1.0 - FB->GetValTo()) / 4.0;

	Diags->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StartShake);
	Diags->OnShow.AddUniqueDynamic(this, &ALStep::DlgShow); // to fbincrease
	Diags->AddId("C1S2.0"); // "i'll use the tape"
}

void ALStepC1S002::StartShake() {
	Diags->OnDone.RemoveDynamic(this, &ALStepC1S002::StartShake);
	Diags->OnShow.RemoveDynamic(this, &ALStep::DlgShow);
	
	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StartCameraShake(ShakeClass);

	// FB->SetVal(1); // bump to max

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ALStepC1S002::ShakeStarted, 2);
}

void ALStepC1S002::ShakeStarted() {
	Diags->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StopShake);
	Diags->AddId("C1S2.1"); // it got worse
}

void ALStepC1S002::StopShake() {
	Diags->OnDone.RemoveDynamic(this, &ALStepC1S002::StopShake);

	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StopAllCameraShakes(true); // immediate needed since it has no end

	FB->SetVal(.85);

	Finish();
}
