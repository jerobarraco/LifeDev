// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"

#include "DefaultCameraShakeBase.h"

#include "Diags/Diags.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;

	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));

	// static ConstructorHelpers::FClassFinder<UClass> CChar(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Ghost/Ghost_NS.Ghost_NS"));
	CharClass = ALNPC01::StaticClass();
	
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CShake(TEXT("/Game/LifeDev/Game/Chaps/01/Steps/02/CamShake_B"));
	ShakeClass = CShake.Succeeded()? CShake.Class.Get() : UDefaultCameraShakeBase::StaticClass();
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	Flashback = GetWorld()->GetSubsystem<UFlashback>();
	SpawnGhosts();
}

void ALStepC1S002::SpawnGhosts() {
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StartShake);
	Dialogs->AddId("C1S2.0"); // "i'll use the tape"
	
	Ghosts = Cast<AGhosts>(GetWorld()->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostLocation);
		Ghosts->SetPlaying(true);
	}
	Flashback->SetMax(1); // reset to 1 since we will change it several times here
	Flashback->SetVal(.75); // was already clamped to .7 on c1s0, so it cant be bigger
}

void ALStepC1S002::StartShake() {
	Dialogs->OnDone.RemoveAll(this);

	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StartCameraShake(ShakeClass);

	ALMusicMan::SetRainS(World, true);

	Flashback->SetVal(1); // bump to max

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ALStepC1S002::ShakeStarted, 2);
}

void ALStepC1S002::ShakeStarted() {
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StopShake);
	Dialogs->AddId("C1S2.1"); // it got worse
}

void ALStepC1S002::StopShake() {
	Dialogs->OnDone.RemoveAll(this);
	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ALStepC1S002::DestroyGhosts, 2);
}

void ALStepC1S002::DestroyGhosts() {
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
	}
	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ALStepC1S002::GhostDestroyed, 2);
}

void ALStepC1S002::GhostDestroyed() {
	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StopAllCameraShakes(true); // immediate needed since it has no end
	Flashback->SetVal(.85);

	if (IsValid(Ghosts)) {
		Ghosts->Destroy();
		Ghosts = nullptr;
	}
	Finish();
}
