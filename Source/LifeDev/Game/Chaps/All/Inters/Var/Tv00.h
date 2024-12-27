// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Tv00.generated.h"

class UCLSounder;
class UCLSignificance;
class UCRandomizer;
class UCQuickMesh;

// TODO TObjectPtr

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
	UCLSounder* Noise = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCRandomizer* RndCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCAnimatorMix* AnimCrt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCQuickMesh* Frame = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCQuickMesh* Glass = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCQuickMesh* Crt = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCLSignificance* Sig = nullptr;
};
