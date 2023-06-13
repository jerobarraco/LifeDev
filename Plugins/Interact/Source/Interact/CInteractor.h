// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "InteractTypes.h"

#include "CInteractor.generated.h"

class UCInteract;
class UInteractorUI;
class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractBegin, UCInteract*, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractEnd, UCInteract*, Comp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractToggle, bool, On, UCInteract*, Comp);

// Will be interacting with interact objects.
// Subclass of scene component so you can attach it and aim from there
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteractor: public USceneComponent {
public:
	GENERATED_BODY()

	UCInteractor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool Enabled);
	
	UFUNCTION(BlueprintCallable)
	void TryTrigger();

	// don't call directly the character will. doesn't deal with the inventory.
	UFUNCTION(BlueprintCallable)
	EItemUseResult TryUseItem(const FName& Name) const;

	// you need to set this once.
	UFUNCTION(BlueprintCallable, Category=SetUp)
	static void SetCollisionChannel(ECollisionChannel Channel) {
		InteractChannel = Channel;
	}

	// the currently hovered component. can be null.
	UFUNCTION(BlueprintCallable)
	UCInteract* GetInterComp() { return InterComp; }

	// The max length to trace for
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float TraceLen = 500.0;

	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FOnInteractToggle OnToggle;
	// triggered when it begins hovering an interact
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FOnInteractBegin OnBegin;
	// triggered when ends hovering an interact
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FOnInteractEnd OnEnd;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ends an interaction
	void DoEnd();
	// attempts to trigger a start
	void DoStart(UCInteract* Component);

	inline static ECollisionChannel InteractChannel = ECC_Visibility;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UArrowComponent* IArrow = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UCInteract* InterComp = nullptr;
};
