// Copyright Jerónimo Barraco-Mármol

#include "Door03.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ADoor03::ADoor03():Super() {
	Mesh->SetRelativeLocation(FVector(5,-5,0));
	Mesh->SetRelativeRotation(FRotator(0,180,0));
	Anim->TEnd.SetRotation(FRotator(0, 90, 0).Quaternion());
}
