// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LNPCH.generated.h"

USTRUCT(BlueprintType)
struct FNPCHPose {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InteractOrg = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector InteractExt = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Pelvis;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Torso;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform Head;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmL1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmL2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmR1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ArmR2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegL1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegL2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegR1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform LegR2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform FootL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform FootR;
};

// Base class for human-like npcs
UCLASS(Blueprintable, BlueprintType)
class ALNPCH: public ALInteract {
	GENERATED_BODY()

public:
	ALNPCH();

	UFUNCTION(BlueprintCallable)
	void SetPose(const FNPCHPose& Pose);

protected:
	// prints the current pose to the console
	UFUNCTION(BlueprintCallable, CallInEditor)
	void PrintPose();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	FNPCHPose PoseBase;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Head = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Torso = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Pelvis = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ArmL1 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ArmR1 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ArmL2 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ArmR2 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LegL1 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LegR1 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LegL2 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LegR2 = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> FootL = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> FootR = nullptr;
};
