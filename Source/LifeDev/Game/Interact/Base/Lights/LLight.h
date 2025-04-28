// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LLight.generated.h"

class UCLSounder;
class UCLSignificance;
class UCRandomizer;

// Base class for light actors. doesn't include the light itself. see LLight00
// notice that it expects to use the Anim for animations.
// It manages UseAnim with the A_STROBE flag. AnimUpdate will be called anyway on state change.
// Remember to set cast shadow on the correct meshes
// Off = 0 (Closed) On = 1 (Open)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight: public ALInteract {
	GENERATED_BODY()

public:
	ALLight();

	// don't call before begin play. Sets the new threshold to flicker.
	// It does checks for A_STROBE flag and will not set it otherwise.
	// It will subscribe to many delegates.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetFBFlicker(const float NewFBFlicker);

	// Unbinds from all the flicker delegates. Disables UseAnim as well.
	// it doesn't clear the FlickerOnFB value.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void StopFBFlicker();
	virtual void SetStateNow_Implementation(const int32 NewState, const bool UseSFX = false, const bool UseParts = false) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetState_Implementation(const int32 NewState) override;

	// used to animate the light. in case of no A_Strobe this is called only once with the final value
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void AnimUpdate(const float P, const float A);

	UFUNCTION() // bind
	void SetFB(const float Value);
	UFUNCTION() // bind
	void FeatUpdated(const EFeat Feat, const bool bEnabled);
	UFUNCTION()
	void FlickerBegin();
	UFUNCTION()
	void FlickerEnd();
	// flickers when fb is >= this value. <0 means disabled.
	// 0 means always
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	float FlickrOnFB = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntensityMax = 12;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCRandomizer> Rnd = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCLSignificance> Sig = nullptr;

	// used to produce the flicker sfx. not using the regular SFX because they could cancel each other.
	// the sound needs to be assigned. and a float param will be set for "Duration"
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCLSounder> SFX_Flicker = nullptr;
};
