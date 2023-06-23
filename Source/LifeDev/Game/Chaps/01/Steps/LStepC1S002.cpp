// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S002.h"

#include "Niagara/Public/NiagaraComponent.h"
#include "GameplayCameras/Public/DefaultCameraShakeBase.h"

#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"

ALStepC1S002::ALStepC1S002():Super() {
	Name = FName("C1S2");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;

	Root->SetWorldLocation(FVector(-78.576659,736.134006,20.947626));
	Root->SetWorldRotation(FRotator(26.779513,334.411499,19.340760));

	Ghosts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ghosts"));
	Ghosts->SetupAttachment(Root);
	Ghosts->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiagara(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghost_NS.Ghost_NS"));
	Ghosts->SetAsset(CNiagara.Object);
	Ghosts->SetRelativeLocation(FVector(207.355288,0.509086,48.526007));

	// static ConstructorHelpers::FClassFinder<UClass> CChar(TEXT("/Game/LifeDev/Game/Chaps/All/Chars/Ghost/Ghost_NS.Ghost_NS"));
	CharClass = ALNPC01::StaticClass();
	
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>
		CShake(TEXT("/Game/LifeDev/Game/Chaps/1/Steps/02/CamShake_B"));
	ShakeClass = CShake.Succeeded()? CShake.Class.Get() : UDefaultCameraShakeBase::StaticClass();
}

void ALStepC1S002::Start_Implementation() {
	Super::Start_Implementation();
	SpawnGhosts();
}

void ALStepC1S002::SpawnGhosts() const {
	Ghosts->Activate(true);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC1S002::StartShake);
	Dialogs->AddId("C1S2.0"); // i'll use the music
	// Ghosts->ResetSystem();
}

void ALStepC1S002::StartShake() {
	Dialogs->OnDone.RemoveAll(this);

	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StartCameraShake(ShakeClass);

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ALStepC1S002::ShakeStarted, 5);
}

void ALStepC1S002::ShakeStarted() {
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC1S002::DestroyGhosts);
	Dialogs->AddId("C1S2.1"); // it got worse
}

void ALStepC1S002::DestroyGhosts() {
	Dialogs->OnDone.RemoveAll(this);
	Ghosts->Deactivate();
	FTimerHandle H;
	GetWorld()->GetTimerManager().SetTimer(H, this, &ALStepC1S002::GhostDestroyed, 3);
}

void ALStepC1S002::GhostDestroyed() {
	UWorld* const World = GetWorld();
	APlayerController* const Controller = World->GetFirstPlayerController();
	TObjectPtr<APlayerCameraManager> CameraManager = Controller->PlayerCameraManager;
	CameraManager->StopAllCameraShakes(true); // immediate needed since it has no ed
	Finish();
}
