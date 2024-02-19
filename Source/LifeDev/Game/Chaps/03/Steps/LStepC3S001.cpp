// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S001.h"

#include "Camera/CameraComponent.h"

ALStepC3S001::ALStepC3S001():Super() {
	Name = FName("C3S1");
	UseFade = false;
	DlgId = FName("C3S1"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;

	
	Cam->SetConstraintAspectRatio(true);
	Cam->AspectRatio = 2;

	// TODO raise the fb with each dialog
}
