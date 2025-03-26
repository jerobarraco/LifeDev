// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Tv00.h"

#include "TvI00.generated.h"

// Tv00 Instance 00 (ch01-room01)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATvI00: public ATv00 {
	GENERATED_BODY()

public:
	ATvI00();

protected:
	virtual void SetState_Implementation(const int32 NewState) override;
};
