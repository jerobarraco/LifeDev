// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Components/SpotLightComponent.h"
#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Interact/CLSignificance.h"

#include "StreetLight.generated.h"

// Base class for StreetLights
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AStreetLight: public AActor {
	GENERATED_BODY()

public:
	AStreetLight();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void AnimUpdate(float Progress, float Alpha);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntensityMax = 5;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USpotLightComponent* Spot = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCAnimator* Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCRandomizer* Randomizer = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCLSignificance* Sig = nullptr;
};
