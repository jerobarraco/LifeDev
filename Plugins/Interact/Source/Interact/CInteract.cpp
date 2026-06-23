// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CInteract.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "CInteractor.h"

DEFINE_LOG_CATEGORY_STATIC(LogCInteract, Log, Log);

UCInteract::UCInteract(): Super() {
	// These two seem to work ok. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	UBoxComponent::SetComponentTickEnabled(false);
	
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	bNavigationRelevant = false;
	bCanEverAffectNavigation = false;
	SetGenerateOverlapEvents(false); // nopes because it will create collision issues. 
	SetCanEverAffectNavigation(false);

	UBoxComponent::SetCollisionProfileName(ProfileNone); // Start disabled, in case someone calls setAutoActivate(false) at some point
	UCInteract::SetAutoActivate(false); // read note on the header's bottom.
}

void UCInteract::Trigger() const {
	UE_LOG(LogCInteract, Log, TEXT("%hs Interact triggered. Obj=%s"), __func__,
		*GetNameSafe(GetOwner()));
	// this is synchronous. will call AInteract(Anim, Fade) tryTrigger. and SetText at some point before this function returns

	// no need to set instigator it's already done on hover
	OnTrigger.Broadcast();
}

void UCInteract::Hover(const bool IsHover, APawn* const Inst) const {
	// Apply the hover to the hover mesh AND all its children
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s: IsHover=%i"),
		__func__, *GetNameSafe(this), IsHover);

	SetStencil(IsHover ? HoverStencilID : -1);

	AActor* const Owner = GetOwner();
	// this way kind of sucks. but it's the best. it's important to set before hover and clear after unhover.
	if (IsHover & LIKELY(Owner)) Owner->SetInstigator(Inst); // always before onHover if IsHover
	OnHover.Broadcast(IsHover);
	if (!IsHover & LIKELY(Owner)) Owner->SetInstigator(nullptr); // always null. always after onHover if !IsHover
}

void UCInteract::Look() const {
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s"), __func__, *GetNameSafe(this));
	// no need to set instigator. it's already done on hover
	OnLook.Broadcast();
}

void UCInteract::DeInit() {
	// not very necessary, since they are weak. but will keep the rest clean.
	HoverMesh = nullptr;
	PhysComp = nullptr;
}

bool UCInteract::TryGrab(const bool IsGrab, UCInteractor* const NewParent) {
	UE_LOG(LogCInteract, Log, TEXT("%hs. IsGrab=%i, IsGrabbable=%i, IsGrabbed=%i, NewParent=%s"),
		__func__, IsGrab, IsGrabbable, IsGrabbed, *GetNameSafe(NewParent));
	// avoid stealing the grab
	if (IsGrab & (!IsGrabbable | IsGrabbed)) return false;
	
	IsGrabbed = IsGrab;
	Reparent(IsGrab, NewParent); // note this happens after IsGrabbable return above

	OnGrab.Broadcast(IsGrab, NewParent);
	return true;
}

void UCInteract::Hint(const bool Show) const {
	const AActor* const Owner = GetOwner();
	const APawn* const Instigator = Owner ? Owner->GetInstigator() : nullptr;
	// override stencil if On, and try to restore hover if off
	const int32 StencilID = Show ? HintStencilID :
		(Instigator ? HoverStencilID : -1);
	SetStencil(StencilID);
	OnHint.Broadcast(Show);
}

void UCInteract::SetStencil(int32 StencilID) const {
	UStaticMeshComponent* const HMesh = HoverMesh.Get();
	// wrapped to always trigger the delegate
	if (UNLIKELY(!IsValid(HMesh))) return;

	const bool Show = StencilID >= 0;
	if (!Show) StencilID = 0; // force at 0, save one branch*n 

	TArray<USceneComponent*> Meshes;
	HMesh->GetChildrenComponents(true, Meshes);
	Meshes.Add(HMesh);

	const int32 Num = Meshes.Num();
	for (int32 i = 0; i<Num; ++i) {
		UStaticMeshComponent* const Child = Cast<UStaticMeshComponent>(Meshes[i]);
		if (UNLIKELY(!IsValid(Child))) continue;
		
		Child->SetRenderCustomDepth(Show);
		// To have this working you need to enable the usage of custom stencils on the settings to
		// "Custom depth stencil pass : Enabled WITH STENCIL"
		Child->SetCustomDepthStencilValue(StencilID);
	}
}

void UCInteract::Reparent(const bool IsGrab, UCInteractor* const NewParent) const {
	UE_LOG(LogCInteract, Log, TEXT("%hs, isGrab=%i parent=%p"),
		__func__, IsGrab, NewParent);

	if (PhysComp.Get())
		ReparentPhys(IsGrab, NewParent);
	else
		ReparentActor(IsGrab, NewParent);
}

void UCInteract::ReparentActor(const bool IsGrab, UCInteractor* const NewParent) const {
	UE_LOG(LogCInteract, Log, TEXT("%hs, isGrab=%i parent=%p"), __func__, IsGrab, NewParent);
	AActor* const Actor = GetAttachParentActor();
	if (UNLIKELY(!Actor)) {
		UE_LOG(LogCInteract, Warning, TEXT("%hs, Could not get attached parent actor"), __func__);
		return;
	}
	
	if (IsGrab) {
		if (UNLIKELY(!IsValid(NewParent))){
			UE_LOG(LogCInteract, Warning, TEXT("%hs, Could not get attached parent actor"), __func__);
			return;
		}
		Actor->AttachToComponent(NewParent, FAttachmentTransformRules::KeepWorldTransform);
	} else
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UCInteract::ReparentPhys(const bool IsGrab, const UCInteractor* const NewParent) const {
	UPrimitiveComponent* const PPhysComp = PhysComp.Get();
	if (UNLIKELY(!PPhysComp)) {
		UE_LOG(LogCInteract, Warning, TEXT("%hs: Could not get the physcomp"), __func__);
		return;
	}

	if (IsGrab) {
		if (UNLIKELY(!IsValid(NewParent))) return;

		const UPrimitiveComponent* const PrimParent = NewParent->GrabRoot.Get();
		UPhysicsHandleComponent* const Handler = NewParent->GrabHandler.Get();
		if (UNLIKELY(!(PrimParent && Handler))) {
			UE_LOG(LogCInteract, Warning,
				TEXT("%hs: Could not get the primparent, or constraint"), __func__);
			return;
		}

		Handler->Activate(true);
		static FTransform F;
		F = PPhysComp->GetComponentTransform();
		Handler->GrabComponentAtLocationWithRotation(PPhysComp, NAME_None,
			F.GetLocation(), F.Rotator());
		// PhysComp->WakeAllRigidBodies();
	}
	// the un-grabbing is done by the interactor. (since it has and needs the handler)
}

void UCInteract::SetCollisionEnabledBool(const bool Enabled) {
	// because this is ForceInline, maybe this can be compiled without the "if", when called with a constexpr?
	// SetCollisionEnabled changes the whole profile, so don't use it!
	SetCollisionProfileName(Enabled ? ProfileDefault : ProfileNone);
}

void UCInteract::Deactivate() {
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s"),
		__func__, *GetNameSafe(GetOwner()));
	Super::Deactivate();
	SetCollisionEnabledBool(false);
	SetStencil(-1); // disable stencils. the CInteractor will call Hover(false) anyway.
}

void UCInteract::Activate(const bool bReset) {
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s"),
		__func__, *GetNameSafe(GetOwner()));
	
	Super::Activate(bReset);
	// is ok to just disable the collision and not lock the trigger method,
	// since there are cases where we want to trigger manually.
	SetCollisionEnabledBool(true);
}

void UCInteract::SetActive(const bool bNewActive, const bool bReset) {
	// this is like traveling to the future backwards, i'm warning my future self of a past problem.
	// UE_CLOG(bRegistered && !IsOwnerRunningUserConstructionScript(), LogCInteract, Warning,
	const bool NeedsInit = NeedsInitialization() | OwnerNeedsInitialization();
	if (UNLIKELY(NeedsInit)) {
		UE_LOG(LogCInteract, Warning, TEXT("%hs Skipped during construction. O=%s"),
			__func__, *GetNameSafe(GetOwner()));
		// SetAutoActivate(bNewActive); // no, don't use autoactivate since it can break other logics.
		return;
	}

	Super::SetActive(bNewActive, bReset);
}

// no need to call setcollisionEnabledBool here.
// since it Activate gets called somewhere upon normal gameplay, and we set the collision to disabled on the constructor.
// if the interact starts active, check if you haven't overriden the default value for the collision on the editor's outliner.
// void UCInteract::SetAutoActivate(const bool NewActive) {
	// Super::SetAutoActivate(NewActive);
	// SetCollisionEnabledBool(NewActive);
// }
