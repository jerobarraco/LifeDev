// Copyright Jerónimo Barraco-Mármol

#include "PictureI01.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"

APictureI01::APictureI01():Super() {
	Texts = { NSLOCTEXT("PictureI01", "State0", "Picture") };
	// TriggerDlg = "PIC01_T";
	IsOneShot = true;
	UseAutoActivate = true;
	RewardFlash = .09;
	StateNum = 2;
	UseAnim = true;
	SFXs = { SFXs[0], SFXs[0]}; // expand to 2 states
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat (TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	if (CMat.Succeeded()) Mesh->SetMaterial(0, CMat.Object.Get());
	
	Anim->IsAdditive = false;
	// sets scale. scale ==0 by default since additive is the usual
	Anim->TStart = Anim->TEnd = FTransform::Identity;
	const FRotator Rot(10.,0.000000,5.000000);
	Anim->TStart.SetRotation(Rot.Quaternion());
	Anim->TStart.SetLocation(FVector(5, 0,0));
	IRoot->SetRelativeTransform(Anim->TStart);
	
	Super::SetMobility(EComponentMobility::Movable);
}
