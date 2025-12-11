// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "CLCharArm.h"

UCLCharArm::UCLCharArm() {
	PrimaryComponentTick.bCanEverTick = true;
	
	bEnableCameraRotationLag = true;
	bEnableCameraLag = true;
	TargetArmLength = 0;
	CameraLagMaxDistance = 100;
	bDoCollisionTest = false;
	bUsePawnControlRotation = true;
}
