// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "Interact/Interact.h"

#include "InteractAnim.generated.h"

class UCAnimatorMix;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType, Category="Interact|InteractAnim")
class INTERACT_API AInteractAnim: public AInteract {
	GENERATED_BODY()

public:
	AInteractAnim();

	// Flip-flops depending on the state. Starts closed, then open, then closed.
	// this also controls the "reversed" flag of the anim.
	// If you need to _not_ use the reversed animation then use the "Trans" property.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsEven() const { return State % 2 == 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCAnimatorMix* GetAnim() const {return Anim;}

	virtual void SetStateNow_Implementation(const int32 NewState, const bool UseSFX = false, const bool UseParts = false) override;
	
	// whether it will trigger animations using the Anim component.
	// this gets abused in several situations. like when changing the mobility,
	// when strobe is disabled on lights, on beginplay, and many, many more. :)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	bool UseAnim = true;

	// will deactivate while playing and re-enable after.
	// be careful if you need to disable on trigger or smth
	// SHOULD be ignored if UseAnim == false. but beware of bugs.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	bool DisableWhileAnim = true;

	// the transforms for each state. if this is set it will override the anim values.
	// the isAdditive flag will be respected, but probably won't work nicely.
	// won't affect the reversed flag.
	// mostly used for puzzles.
	// if you need to set material values, i'd recommend overriding SetState_Implementation instead.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	TArray<FTransform> Trans;
	// might be good to rework this, texts, sfx_start and sfx_stop

	// triggered when anim ends. Closed, Open. (for trigger on start see SFXs)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|State")
	TArray<TObjectPtr<USoundBase>> SFX_Stop;

	// When this is triggered, not locked, after anim.
	// Means after the animation is done (if it has any).
	// OnTrigger is preferred if you don't use animations,
	// or you don't REALLY need to wait for the end of the animation.
	// I wouldn't expect this to be super reliable,
	// specially if the user triggers this multiple times while it's animating.
	// DisableWhileAnim will help.
	// Make sure to call parent if you override AnimEnd, or TryTrigger.
	// If the animation is looped, this will get triggered multiple times.
	UPROPERTY(BlueprintAssignable, Transient, Category=SetUp)
	FAInteractOnTrigger OnAnimEnd;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetState_Implementation(const int32 NewState) override;
	virtual bool TryTrigger_Implementation() override;
	virtual void DoTrigger_Implementation() override;

	
	// triggers the animation. checks some flags first.
	void AnimPlay();
	void AnimSet();

	// note there's no animUpdate. because it's quite costly and almost never needed.
	// so it's specified when needed.

	// Called when the animation begins. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction)) // bound
	void AnimBegin();

	// Called when the animation end. It gets called each loop.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction)) // bound
	void AnimEnd();
	
	// The animator, by default set up for the mesh material and iroot.
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorMix> Anim = nullptr;
};


// Whether it should reverse the animation.
// By default, it will not reverse if StateNum <=1. otherwise it depends on IsClosed.
// UFUNCTION(BlueprintCallable, BlueprintPure)
// FORCEINLINE bool IsReversed() const { return StateNum > 1 && IsClosed(); }
