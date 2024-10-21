// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "InteractTypes.h"

#include "CInteractor.generated.h"

class UPhysicsHandleComponent;
class UPhysicsConstraintComponent;
class UCInteract;
class UInteractorUI;
class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractBegin, UCInteract* const, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractEnd, UCInteract* const, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractToggle, bool, On, UCInteract* const, Comp);

// Will be interacting with interact objects.
// Subclass of scene component, so you can attach it and aim from there.
// By default, is not active.
UCLASS(Blueprintable, BlueprintType, Config="Interact", DefaultConfig,
	ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteractor: public USceneComponent {
	GENERATED_BODY()

public:
	UCInteractor(const FObjectInitializer& ObjectInitializer);
	virtual void Deactivate() override;
	virtual void Activate(const bool Reset) override;
	
	// you need to set this once. but only if you need to change the default.
	UFUNCTION(BlueprintCallable, Category=SetUp)
	static void SetCollisionChannel(const ECollisionChannel NewChannel) {
		InteractChannel = NewChannel;
	}

	// Triggers the currently hovered component (if any)
	UFUNCTION(BlueprintCallable)
	void TryTrigger();
	UFUNCTION(BlueprintCallable)
	EItemUseResult TryUseItem(const FName Name) const;

	// the currently hovered component. can be null.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCInteract* GetInterComp() { return InterComp; }

	UFUNCTION(BlueprintCallable)
	bool TryGrab(const bool IsGrab=true);
	
	// The max length to trace for
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=SetUp)
	float TraceLen = 200.0;
	// if <1 it will use a line trace. > will use a box trace
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=SetUp)
	float TraceSize = 2;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TObjectPtr<UPrimitiveComponent> GrabRoot = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TObjectPtr<UPhysicsConstraintComponent> GrabConstraint = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="SetUp|Grab")
	TObjectPtr<UPhysicsHandleComponent> GrabHandler = nullptr;
	
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnInteractToggle OnToggle;
	// triggered when it begins hovering an interact
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnInteractBegin OnBegin;
	// triggered when ends hovering an interact
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FOnInteractEnd OnEnd;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Attempts to trigger on the server
	UFUNCTION(Server, Reliable)
	void SrvTrigger(const UCInteract* const Comp) const;
	
	// ends an interaction
	void DoEnd();
	// attempts to trigger a start
	void DoStart(UCInteract* const Component);

	inline static ECollisionChannel InteractChannel = ECC_Visibility;
	
	// the currently hovered interact component
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UCInteract> InterComp = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UCInteract> GrabbedComp = nullptr;
};
