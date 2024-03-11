// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Tv00.h"

#include "TvI01.generated.h"

// Tv00 Instance 00 (ch01-room01)
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATvI01: public ATv00 {
	GENERATED_BODY()

public:
	ATvI01();

protected:
	virtual void DoTrigger_Implementation() override;

	UFUNCTION()
	void AfterOn();
	UFUNCTION()
	void AfterDlg();
};
