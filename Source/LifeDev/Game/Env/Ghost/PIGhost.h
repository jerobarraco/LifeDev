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
	virtual void PostDuplicate(bool Pie) override;
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
