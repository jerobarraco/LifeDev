// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"

#include "CInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractOnHover, bool, IsOn);

// You will need to define a collision profile. and set it here.
// Use BlockAllDynamic as last resource. since that will collide with objects and might create issues.
// All you need is a profile that blocks the same channel you set on CInteractor::SetCollisionChannel
// i recommend visibility

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteract: public UBoxComponent {
public:
	GENERATED_BODY()

	UCInteract(const FObjectInitializer& ObjectInitializer);

	virtual void PostCDOContruct() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool IsEnabled);

	// you need to set this once.
	UFUNCTION(BlueprintCallable, Category=SetUp)
	static void SetCollisionProfile(const FName& Name) {
		CollisionProfile = Name;
	}

	// used by the interactor. don't call directly. subscribe to OnTrigger.
	UFUNCTION()
	void Trigger() const;
	
	// used by the interactor. don't call directly. subscribe to the OnHover delegate.
	UFUNCTION()
	void Hover(bool IsHover) const;

	UFUNCTION()
	void DeInit();
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnTrigger OnTrigger;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnHover OnHover;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FText Text = FText::GetEmpty();

	inline static FName CollisionProfile = "Interact";
	
	// Mesh to automatically highlight, if any.
	UPROPERTY(BlueprintReadWrite, Category=SetUP)
	UStaticMeshComponent* HoverMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPostProcessComponent* PostProcess = nullptr;
};
