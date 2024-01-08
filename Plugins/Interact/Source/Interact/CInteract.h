// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once
#include "Components/BoxComponent.h"

#include "CInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractOnHover, bool, IsOn);

// Helps define an interaction volume
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCInteract: public UBoxComponent {
	GENERATED_BODY()

public:
	UCInteract();

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool IsEnabled);

	// Sets the default collision channel for new instances. only need to call once. by default will use "Interact"
	UFUNCTION(BlueprintCallable, Category=SetUp)
	static void SetCollisionProfile(const FName& Name) {
		CollisionProfile = Name;
	}

	// used by the interactor (hence public). don't call directly. subscribe to OnTrigger.
	UFUNCTION()
	void Trigger() const;
	
	// used by the interactor (hence public). don't call directly. subscribe to the OnHover delegate.
	UFUNCTION()
	void Hover(bool IsHover) const;

	UFUNCTION()
	void DeInit();
	
	inline static FName CollisionProfile = "Interact";

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnTrigger OnTrigger;
	
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FInteractOnHover OnHover;
	
	// Text to be displayed on interaction
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	FText Text = FText::GetEmpty();
	
	// Mesh to automatically highlight, if any.
	UPROPERTY(BlueprintReadWrite, Category=SetUP)
	UStaticMeshComponent* HoverMesh = nullptr;
};
