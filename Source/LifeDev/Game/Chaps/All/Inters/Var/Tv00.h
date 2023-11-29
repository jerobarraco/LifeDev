// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Tv00.generated.h"

class UCSounder;
class UCRandomizer;
class UCQuickMesh;
class UCSignificance;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATv00: public ALInteract {
	GENERATED_BODY()

public:
	ATv00();

protected:
	virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* Noise = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCRandomizer* RndCrt = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimatorMix* AnimCrt = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Frame = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Glass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Crt = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSignificance* Sig = nullptr;
};
