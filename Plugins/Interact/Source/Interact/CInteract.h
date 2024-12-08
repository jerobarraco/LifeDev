// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once
#include "Components/BoxComponent.h"

#include "CInteract.generated.h"

class UCInteractor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractOnHover, const bool, IsOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractOnGrab, const bool, IsGrab, UCInteractor* const, NewParent);

// Base component for interactions
// Defines a volume where the interaction is triggered.
// Set the relative position on the viewport, (but avoid changing the scale). Also set the box extent. 
// By default, it will start active, since there's a ue issue around that.
// To change it you'll need to do it on BeginPlay.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
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
	inline static const FName ProfileNone = "NoCollision";
	
	// whether the parent actor can be grabbed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Grab")
	bool IsGrabbable = false;
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FText Text = FText::GetEmpty();
	
	// Mesh to automatically highlight, if any.
	// will write a custom render stencil value 255.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UStaticMeshComponent> HoverMesh = nullptr;

	// Component to enable/disable physics on grabbing.
	// When this is Grabbable, and the mesh is simulating physics, the mesh should be set here.
	// Potentially on construction (otherwise beginplay).
	// If you're using the Interact actor, and the mesh is set to "Simulate Physics",
	// this variable will be set (and overriden) on begin play automatically.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Grab")
	TObjectPtr<UPrimitiveComponent> PhysComp = nullptr;

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
	void Reparent(const bool bIsGrab, UCInteractor* const NewParent);
	void ReparentActor(const bool IsGrab, UCInteractor* const NewParent) const;
	void ReparentPhys(const bool IsGrab, const UCInteractor* const NewParent) const;
	FORCEINLINE void SetCollisionEnabledBool(const bool Enabled);

#pragma region Interactor
	/// interactor
	// used by the interactor (hence public). don't call directly. subscribe to OnTrigger.
	void Trigger() const;
	// used by the interactor (hence public). don't call directly. subscribe to the OnHover delegate.
	void Hover(bool IsHover) const;
	// used by the interactor (hence public). don't call directly. subscribe to the OnHover delegate.
	void DeInit();
	// used by the interactor (hence public). don't call directly. subscribe to the OnHover delegate.
	bool TryGrab(const bool IsGrab, UCInteractor* const NewParent);

	inline static FName Profile = ProfileInteract;
	friend class AInteract;
	friend class UCInteractor;
#pragma endregion 

	bool IsGrabbed = false;
};
