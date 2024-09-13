// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CInteract.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/BoxComponent.h"

#include "JUtils/Net/JNetUtils.h"

#include "CInteractor.h"

DEFINE_LOG_CATEGORY_STATIC(LogCInteract, Log, Log);

UCInteract::UCInteract(): Super() {
	// these two seem to work ok. but keep an eye on.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	UBoxComponent::SetComponentTickEnabled(false);
	
	CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	bNavigationRelevant = false;
	bCanEverAffectNavigation = false;
	// nopes because it will create collision issues.
	SetGenerateOverlapEvents(false);
	SetCanEverAffectNavigation(false);

	Super::SetAutoActivate(true); // important to fix the issue with interact starting inactive.
	
	UBoxComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly); // it's already on the collision profile yay
	UBoxComponent::SetCollisionProfileName(CollisionProfile);
}

void UCInteract::Trigger() const {
	// server or client. depends on ShouldReplicate, the IT_REPLICATED flag.
	// And it's decided by the CInteractor (since it's owned by the controller and can call RPCs).
	
	UE_LOG(LogCInteract, Log, TEXT("%hs Interact triggered. IsServer=%i Role=%s Obj=%s"),
		__func__, JU_IsServerSide,
		*UEnum::GetValueAsString(GetOwnerRole()), *GetNameSafe(GetOwner()));
	// this is synchronous. will call AInteract(Anim, Fade) tryTrigger. and SetText at some point before this function returns
	OnTrigger.Broadcast();
}

void UCInteract::Hover(bool IsHover) const {
	// Apply the hover to the hover mesh AND all its children
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s: IsHover=%i Server=%i, Role=%s."),
		__func__, *GetNameSafe(this), IsHover,
		JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));

	// wrapped to always trigger the delegate
	if (IsValid(HoverMesh)) {
		TArray<USceneComponent*> Children;
		HoverMesh->GetChildrenComponents(true, Children);
		Children.Add(HoverMesh);
		const int32 Num = Children.Num();
		for (int32 i = 0; i<Num; ++i) {
			UStaticMeshComponent* const Child = Cast<UStaticMeshComponent>(Children[i]);
			if (!IsValid(Child)) continue;
			
			Child->SetRenderCustomDepth(IsHover);
			// To have this working you need to enable the usage of custom stencils on the settings to
			// "Custom depth stencil pass : Enabled WITH STENCIL"
			Child->SetCustomDepthStencilValue(IsHover?255:0);
		}
	}

	OnHover.Broadcast(IsHover);
}

void UCInteract::DeInit() {
	HoverMesh = nullptr; // free resources to get gcd
}

bool UCInteract::TryGrab(bool IsGrab, UCInteractor* NewParent) {
	UE_LOG(LogCInteract, Log, TEXT("%hs. IsGrab=%i, IsGrabbable=%i, NewParent=%p"),
		__func__, IsGrab, IsGrabbable, NewParent);
	if (IsGrab) {
		if (!IsGrabbable) return false;
		// small patch to avoid stealing the grab
		// might do something better later to allow to steal the grab, or might not.
		IsGrabbable = false;
	} else {
		// i know this has the sideeffect that if you call trygrab(false) on something not grabbed.
		// that also had isgrabbable to false, it will set it to true.
		IsGrabbable = true;
	}

	Reparent(IsGrab, NewParent); // note this happens after IsGrabbable return above
	
	// if (IsGrab && !IsGrabbable) return false;
	OnGrab.Broadcast(IsGrab, NewParent);
	return true;
}

bool UCInteract::WillReplicate() const {
	return ShouldReplicate && JU_IsServerSide;
}

void UCInteract::BeginPlay() {
	Super::BeginPlay();

	// even though the SetReplicated function is called on server only and after BeginPlay,
	// it seems that CInteractor is successfully able to call GetIsReplicated.
	if (WillReplicate()) {
		SetIsReplicated(true);
		if (IsGrabbable)
			UE_LOG(LogCInteract, Warning, TEXT("%hs Component set to replicate and grababble. That's not supported. Owner=%s"),
				__func__, *GetNameSafe(GetOwner()));
	} else if (JU_IsServerSide)
		SetIsReplicated(false);

	// this is a patch. since by default it starts as disabled on the server, but enabled on the client (wtf)
	Activate(true);
	// TODO test on standalone and non-replicated
}

void UCInteract::Reparent(bool IsGrab, UCInteractor* NewParent) {
	UE_LOG(LogCInteract, Log, TEXT("%hs, isGrab=%i parent=%p"), __func__, IsGrab, NewParent);

	if (PhysComp)
		ReparentPhys(IsGrab, NewParent);
	else
		ReparentActor(IsGrab, NewParent);
}

void UCInteract::ReparentActor(bool IsGrab, UCInteractor* NewParent) {
	UE_LOG(LogCInteract, Log, TEXT("%hs, isGrab=%i parent=%p"), __func__, IsGrab, NewParent);
	AActor* const Actor = GetAttachParentActor();
	if(!Actor) {
		UE_LOG(LogCInteract, Warning, TEXT("%hs, Could not get attached parent actor"), __func__);
		return;
	}
	
	if (IsGrab) {
		if(!IsValid(NewParent)) {
			UE_LOG(LogCInteract, Warning, TEXT("%hs, Could not get attached parent actor"), __func__);
			return;
		}
		Actor->AttachToComponent(NewParent, FAttachmentTransformRules::KeepWorldTransform);
	} else
		Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UCInteract::ReparentPhys(bool IsGrab, UCInteractor* NewParent) {
	if (!PhysComp) {
		UE_LOG(LogCInteract, Warning, TEXT(" %hs Could not get the physcomp"), __func__);
		return;
	}

	if (IsGrab) {
		if (!IsValid(NewParent)) return;

		const UPrimitiveComponent* const PrimParent = NewParent->GrabRoot;
		UPhysicsHandleComponent* const Handler = NewParent->GrabHandler;
		if (!(PrimParent && Handler)) {
			UE_LOG(LogCInteract, Warning, TEXT(" Could not get the primparent, or constraint"));
			return;
		}

		static FTransform F;
		F = PhysComp->GetComponentTransform();
		Handler->GrabComponentAtLocationWithRotation(PhysComp, NAME_None, F.GetLocation(), F.Rotator());
		// PhysComp->WakeAllRigidBodies();
	}
	// the un-grabbing is done by the interactor. (since it has and needs the handler)
}

void UCInteract::Deactivate() {
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s: Server=%i Role=%s"),
		__func__, *GetNameSafe(GetOwner()),
		JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));
	Super::Deactivate();
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCInteract::Activate(bool bReset) {
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s: Server=%i Role=%s"),
		__func__, *GetNameSafe(GetOwner()),
		JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()));
	
	Super::Activate(bReset);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UCInteract::OnRep_IsActive() {
	Super::OnRep_IsActive();

	const bool NewActive = IsActive();
	UE_LOG(LogCInteract, Log, TEXT("%hs: %s: Server=%i Role=%s IsActive=%i"),
		__func__, *GetNameSafe(GetOwner()),
		JU_IsServerSide, *UEnum::GetValueAsString(GetOwnerRole()), NewActive);

	// technically this only executes on the clients. but i rather be safe. or we can cause a loop and stack overflow.
	if (JU_IsServerSide) return;

	// force the collision stuff. and anything else i add on the future.
	SetActive(NewActive, false);

	// IMNSHO variable replication is quite lame, specially the onrep.
	// would be much better to have a 3rd function type that's reliable yet slow (like variables).
	// e.g. Reliable, Unreliable, ReliableSlow
	// the only reason to use var replication instead of rpcs is performance or for vars with no side-effects.
}
