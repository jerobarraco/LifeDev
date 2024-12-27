// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Tv00.generated.h"

class UCLSounder;
class UCLSignificance;
class UCRandomizer;
class UCQuickMesh;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATv00: public ALInteract {
	GENERATED_BODY()

public:
	ATv00();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetState_Implementation(const int32 NewState) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCLSounder> Noise = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCRandomizer> RndCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCAnimatorMix> AnimCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCQuickMesh> Frame = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCQuickMesh> Glass = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCQuickMesh> Crt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCLSignificance> Sig = nullptr;
};
