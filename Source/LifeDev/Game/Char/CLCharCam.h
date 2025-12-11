// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"

#include "CLCharCam.generated.h"

enum class EFeat : uint8;

// LifeDev Game character camera
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API UCLCharCam : public UCameraComponent {
	GENERATED_BODY()
public:
	UCLCharCam();
	
	// Called for the internal init
	UFUNCTION(Blueprintable, BlueprintNativeEvent)
	void Init();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void SetFB(const float Value);
	UFUNCTION()
	void FeatUpdateVisual(const EFeat Feat, const bool bEnabled);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float FOVFoxy = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float FOVMin = 90;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float FOVMax = 140;

	bool UseFeatFOV = true;
};
