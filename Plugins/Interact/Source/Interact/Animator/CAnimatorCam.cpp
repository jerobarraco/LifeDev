#include "CAnimatorCam.h"

#include "Kismet/KismetMathLibrary.h"

// https://forums.unrealengine.com/t/make-the-camera-look-at-a-ball/345097/6?u=nande

void UCAnimatorCam::Begin_Implementation() {
	Super::Begin_Implementation();

	Controller = GetWorld()->GetFirstPlayerController();
	const TObjectPtr<APlayerCameraManager> CamManager = Controller->PlayerCameraManager;
	CamRotStart = CamManager->GetCameraRotation();
	const FVector& CamStart = CamManager->GetCameraLocation();
	CamRotEnd = UKismetMathLibrary::FindLookAtRotation(CamStart, Target);
}

void UCAnimatorCam::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);
	if (!IsValid(Controller)) return;
	// https://forums.unrealengine.com/t/how-to-use-lerp-rotator-in-c/547921/5?u=nande
	const FRotator& Cur = FQuat::Slerp(
		CamRotStart.Quaternion(), CamRotEnd.Quaternion(), Alpha).Rotator();
	Controller->SetControlRotation(Cur);
}
