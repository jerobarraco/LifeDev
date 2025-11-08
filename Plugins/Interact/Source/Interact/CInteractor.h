// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CInteractor.generated.h"

class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class UCInteract;
class UInteractorUI;
class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractHover, bool, On, UCInteract* const, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractTrigger, const UCInteract* const, Comp);

// Will be interacting with interact objects.
// Subclass of scene component, so you can attach it and aim from there.
// By default, is not active.
UCLASS(Blueprintable, BlueprintType, Config=Interact, DefaultConfig,
	ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteractor: public USceneComponent {
	GENERATED_BODY()

public:
	UCInteractor(const FObjectInitializer& ObjectInitializer);
	virtual void Deactivate() override;
	virtual void Activate(const bool Reset) override;

	// you need to set this once. but only if you need to change the default.
	UFUNCTION(BlueprintCallable, Category=SetUp)
	void SetChannel(const ECollisionChannel Chan);

	// Triggers the currently hovered component (if any)
	UFUNCTION(BlueprintCallable)
	void TryTrigger();

	// the currently hovered component. can be null.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCInteract* GetHoverComp() const { return HoverComp.Get(); }

	UFUNCTION(BlueprintCallable)
	bool TryGrab(const bool IsGrab=true);
	
	// The max length to trace for
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=SetUp)
	float TraceLen = 200.0;
	// if <1 it will use a line trace. > will use a box trace
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=SetUp)
	float TraceSize = 2;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TWeakObjectPtr<UPrimitiveComponent> GrabRoot = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TWeakObjectPtr<UPhysicsConstraintComponent> GrabConstraint = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TWeakObjectPtr<UPhysicsHandleComponent> GrabHandler = nullptr;

	// triggered when an interact is un/hovered. It tries to pass the interact.
	// But if it was destroyed, it will be null.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnInteractHover OnHover;

	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnInteractTrigger OnTrigger;

protected:
	UFUNCTION(BlueprintCallable, Category=SetUp)
	void SetTrace(const ETraceTypeQuery Trace);

	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType,
		FActorComponentTickFunction* const ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// ends an interaction
	void DoEnd();
	// attempts to trigger a start
	void DoStart(UCInteract* const Component);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=SetUp)
	TEnumAsByte<ECollisionChannel> Channel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TEnumAsByte<ETraceTypeQuery> TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	// inline static ECollisionChannel InteractChannel = ECC_Visibility;

	// stores whether this is supposedly hovering something. nothing that HoverComp could be null if it gest destroyed.
	// the whole point of this is to detect that situation.
	bool IsHovering = false;
	// the currently hovered interact component
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TWeakObjectPtr<UCInteract> HoverComp = nullptr;
	// the currently grabbed interact component
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TWeakObjectPtr<UCInteract> GrabbedComp = nullptr;
};
