// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Components/BoxComponent.h"

#include "CInteract.generated.h"

class UCInteractor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractOnHover, const bool, IsOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractOnGrab, const bool, IsGrab, UCInteractor* const, NewParent);

/*
this comp is inactive by default because
* it's a problem for performance having stuff enabled by default
* by having it off by default. and controlling (myself) WHEN/if it's on:
* * it makes "race conditions" less prone.
* * it makes it "fail-safe".
* due to the way ue initializes things i can only set enabled by default on actor constructor. (this is the main issue)
* * this prevents me from EXPOSING this to bps. so i can't change it on bps.
* * UE already activates the component (twice) during startup if enabled by default.
* * which means if i want to try to set something deactive by default it will first activate, then deactivate.
* * and that's not up to my standards in programming.
* i tend to (want to) reuse the interacts as stub objects on the level, so i tend to have them deactivated quite a lot.
* * the reason for this is to speed up development. and if i were to need to have a child class only to deactivate, it would negate that benefit.
* also because my game is somewhat of a puzzle/point&click. most items have a chain of interaction that makes almost 99% of them inactive by default.

Important: if there are issues with an interact always activating by default:
go to outline and ensure that the Collision profile for the CInteract comp is reset to default (none).
* 
*/

// Base component for interactions
// Defines a volume where the interaction is triggered.
// Set the relative position on the viewport, (but avoid changing the scale). Also set the box extent. 
// By default, it will start INACTIVE.
// if using an AInteract, toggle UseAutoActivate, don't call SetAutoActivate.
// but you can call it if not using an AInteract.
UCLASS(Blueprintable, BlueprintType,
	ClassGroup=(Interact), meta=(BlueprintSpawnableComponent),
	Config=Interact, DefaultConfig)
class INTERACT_API UCInteract: public UBoxComponent {
	GENERATED_BODY()

public:
	UCInteract();

	virtual void Deactivate() override;
	virtual void Activate(const bool bReset=false) override;
	virtual void SetActive(const bool bNewActive, const bool bReset = false) override;

	// Sets the default collision channel for new instances. only need to call once. by default will use "Interact"
	UFUNCTION(BlueprintCallable, Category=SetUp, meta=(AdvancedDisplay))
	static void SetDefaultCollisionProfile(const FName& Name) { Profile = Name; }

	inline static const FName ProfileInteract = "Interact";
	inline static const FName ProfileHinted = "Hinted";
	inline static const FName ProfileNone = "NoCollision";
	
	// whether the parent actor can be grabbed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Grab")
	bool IsGrabbable = false;
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FText Text = FText::GetEmpty();
	
	// ID to use when writing to the custom depth stencil. Only if HoverMesh is set.
	// Default can be changed on the configs.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp",
		meta=(ClampMin=0, ClampMax=255), Config)
	int32 HoverStencilID = 255;
	// ID to use when writing to the custom depth stencil during hint time. Only if HoverMesh is set.
	// Default can be changed on the configs.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp",
		meta=(ClampMin=0, ClampMax=255), Config)
	int32 HintStencilID = 254;
	
	// Mesh to automatically highlight, if any.
	// will write a custom render stencil value HoverStencilID (255).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TWeakObjectPtr<UStaticMeshComponent> HoverMesh = nullptr;

	// Component to enable/disable physics on grabbing.
	// When this is Grabbable, and the mesh is simulating physics, the mesh should be set here.
	// Potentially on construction (otherwise beginplay).
	// If you're using the Interact actor, and the mesh is set to "Simulate Physics",
	// this variable will be set (and overriden) on begin play automatically.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Grab")
	TWeakObjectPtr<UPrimitiveComponent> PhysComp = nullptr;

	// When this is triggered
	// If this is replicated, this will execute only on server.
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Delegates")
	FInteractOnTrigger OnTrigger;
	
	// When this is being hovered on/off. Not replicated.
	// Always executes on the client that triggered this,
	// The triggering CInteractor is always an AutonomousProxy not a SimulatedProxy.
	// Though THIS Cinteract could be a SimulatedProxy. 
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Delegates")
	FInteractOnHover OnHover;
	
	// When the object is being grabbed or released. Not replicated
	// Always executes on the client that triggered this,
	// The triggering CInteractor is always an AutonomousProxy not a SimulatedProxy.
	// Though THIS Cinteract could be a SimulatedProxy.
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Delegates")
	FInteractOnGrab OnGrab;

protected:
	void Reparent(const bool bIsGrab, UCInteractor* const NewParent) const;
	void ReparentActor(const bool IsGrab, UCInteractor* const NewParent) const;
	void ReparentPhys(const bool IsGrab, const UCInteractor* const NewParent) const;
	FORCEINLINE void SetCollisionEnabledBool(const bool Enabled);
#pragma region Interactor
	/// interactor
	// used by the interactor. don't call directly. subscribe to OnTrigger.
	void Trigger() const;
	// un/hovers. Inst=the instigator, will always be set to null on unhover.
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	void Hover(const bool IsHover, APawn* const Inst) const;
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	void DeInit();
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	bool TryGrab(const bool IsGrab, UCInteractor* const NewParent);
	void Hint(const bool Show) const;
	// <0 will disable stencil
	void SetStencil(int32 StencilID) const;
	
	inline static FName Profile = ProfileInteract;
	friend class AInteract;
	friend class UCInteractor;
#pragma endregion 

	bool IsGrabbed = false;
};
