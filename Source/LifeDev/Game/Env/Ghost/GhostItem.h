// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "GhostItem.generated.h"

class UCLSignificance;
class UCAnimator;
class UCAnimatorMix;
class UCQuickMesh;
class UCGhostAxis;
class UMaterialInterface;

// PoolItemGhost
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class AGhostItem : public AActor {
	GENERATED_BODY()

public:
	AGhostItem();

	UFUNCTION(BlueprintCallable)
	void SetActive(const bool Act, const bool Reset = false);

	UFUNCTION()
	void Return();
	void FadeAndReturn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostDuplicate(const bool Pie) override;
	virtual void Reset() override;

	void SetReturnTimer();
	
	UFUNCTION()
	void BaseUp(const float Progress, const float Alpha);

	UFUNCTION()
	void PosUpX(const float Output, const float NewValue);
	UFUNCTION()
	void PosUpY(const float Output, const float NewValue);
	UFUNCTION()
	void PosUpZ(const float Output, const float NewValue);
	
	UFUNCTION()
	void RotUpX(const float Progress, const float Alpha);
	UFUNCTION()
	void RotUpY(const float Progress, const float Alpha);
	UFUNCTION()
	void RotUpZ(const float Progress, const float Alpha);

	// settings
#pragma region Settings
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> TargetClass;

	// fixed origin from the target. is the origin of the random offset
	UPROPERTY(BlueprintReadWrite, Config)
	FVector OffPos = {0, 0, 30};
	// offset that gets randomized. is a distance from offPos
	UPROPERTY(BlueprintReadWrite, Config)
	FVector OffDist = {200, 200, 75};
	UPROPERTY(BlueprintReadWrite, Config)
	FRotator ActRotOff = {-15, 0, 15};
	UPROPERTY(BlueprintReadWrite, Config)
	float ActRotSpeed = 2.5;
	// UPROPERTY(BlueprintReadWrite, Config)
	// bool Debug = true;
	UPROPERTY(BlueprintReadWrite, Config)
	float LifeTimeMin=20;
	UPROPERTY(BlueprintReadWrite, Config)
	float LifeTimeMax=30;
#pragma endregion

#pragma region Transient
	FVector ActPos;
	FVector ActPosOld;
	FRotator ActRot;
	FRotator OffRot;

	FVector TgtPos;
	FVector AimPos;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<AActor> Target = nullptr;
#pragma endregion

#pragma region CDO
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Mesh = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCGhostAxis> AxisX = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCGhostAxis> AxisY = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCGhostAxis> AxisZ = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimator> AnimBase = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimatorMix> AnimFade = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCLSignificance> Sig = nullptr;
#pragma endregion
};
