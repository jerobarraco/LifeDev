// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Kitchen/Sponge.h"

#include "SpongeI00.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ASpongeI00: public ASponge {
	GENERATED_BODY()

public:
	ASpongeI00();

protected:
	virtual void DoTrigger_Implementation() override;
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<TSoftObjectPtr<ALInteract>> Plates;
};
