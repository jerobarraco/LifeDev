// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Tv00.generated.h"

class UCRandomizer;
class UCQuickMesh;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATv00: public ALInteract {
public:
	GENERATED_BODY()

	ATv00();

protected:
	virtual void BeginPlay() override;
	virtual void Trigger_Implementation() override;

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
};
