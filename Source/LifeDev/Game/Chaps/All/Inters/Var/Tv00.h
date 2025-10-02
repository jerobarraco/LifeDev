// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Core/Consts/ConstSettings.h"
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
	UFUNCTION()
	void FeatUpdV(const EFeat Feat, const bool bEnabled);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCLSounder> Noise = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCRandomizer> RndCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimatorMix> AnimCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Frame = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Glass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Crt = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCLSignificance> Sig = nullptr;
};
