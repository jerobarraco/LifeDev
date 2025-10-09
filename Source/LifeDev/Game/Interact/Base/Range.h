// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "Range.generated.h"

class UCQuickMesh;
class USphereComponent;
class UCAnimatorMix;

// Base class for Range actors
UCLASS(Blueprintable, BlueprintType, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ARange: public AActor {
public:
	GENERATED_BODY()

	ARange();

	// lamest lame thing. to test in editor
	UFUNCTION(BlueprintCallable)
	void Trigger();

	static constexpr float DefScaleMax = 20;
	UFUNCTION(BlueprintCallable)
	void SetScaleMax(const float Scale);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetScaleMax() const { return ScaleMax; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UMaterialInterface> Mat = nullptr;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* const Cmp, AActor* const OtherActor,
		UPrimitiveComponent* const OtherComp, const int32 OtherBodyIndex, const bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void AnimEnd();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Config)
	float ScaleMax = DefScaleMax;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> Root = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> Mesh = nullptr;

	// The animator, by default set up for the mesh material and iroot
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorMix> Anim = nullptr;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> Collider = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UMaterialParameterCollection> MPC = nullptr;
	
	FTimerHandle HRange;
};
