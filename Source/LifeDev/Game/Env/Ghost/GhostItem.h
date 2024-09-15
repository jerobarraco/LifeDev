// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interact/Animator/CAnimator.h"

#include "GhostItem.generated.h"

class UCSignificance;
class UCAnimatorMix;
class UCQuickMesh;
class UCGhostAxis;
class UMaterialInterface;

// PoolItemGhost
UCLASS(Blueprintable, BlueprintType)
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
	virtual void PostDuplicate(bool Pie) override;
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
	TSubclassOf<AActor> TargetClass = APawn::StaticClass();
	
	UPROPERTY(BlueprintReadWrite)
	FVector OffPos = {0, 0, 30};
	UPROPERTY(BlueprintReadWrite)
	FVector OffDist = {75.0, 75, 75};
	UPROPERTY(BlueprintReadWrite)
	FRotator ActRotOff = {-15, 0, 15};
	UPROPERTY(BlueprintReadWrite)
	float ActRotSpeed = 2.5;
	// UPROPERTY(BlueprintReadWrite)
	// bool Debug = true;
	UPROPERTY(BlueprintReadWrite)
	float LifeTimeMin=20;
	UPROPERTY(BlueprintReadWrite)
	float LifeTimeMax=30;
#pragma endregion

#pragma region Transient
	FVector ActPos;
	FVector ActPosOld;
	FRotator ActRot;
	FRotator OffRot;

	FVector TgtPos;
	FVector AimPos;

	UPROPERTY(Transient)
	AActor* Target = nullptr;
#pragma endregion

#pragma region CDO
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Mesh = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCGhostAxis* AxisX = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCGhostAxis* AxisY = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCGhostAxis* AxisZ = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimator* AnimBase = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorMix* AnimFade = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSignificance* Sig = nullptr;
#pragma endregion 
};
