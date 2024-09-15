// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "PIGhost.generated.h"

class UCQuickMesh;
class UCGhostAxis;
class UMaterialInterface;

// PoolItemGhost
UCLASS(Blueprintable, BlueprintType)
class APIGhost : public AActor {
	GENERATED_BODY()

public:
	APIGhost();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostDuplicate(bool Pie) override;
	virtual void Reset() override;
	
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

	/// transient
	FVector ActPos;
	FRotator OffRot;
	FRotator ActRot;

	// settings
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
#pragma endregion 
};
