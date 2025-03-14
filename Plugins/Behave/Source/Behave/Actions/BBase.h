// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once


#include "BBase.generated.h"

// base for action behaviors. the idea is that you make your own and each one handles what it needs.
// a top level action, is considered a goal.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UObject {
	GENERATED_BODY()

public:
	void SetState(int32 TODO) {};
	int32 GetState(){return -1;};
	float Cost(){return 0;};
	float CostSelf(){return 0;};
	bool CanDo(){return false;};
	bool CanDoSelf(){return false; };
	int32 Do(const float DT){return -1;};

	void* OnState;

protected:
	void StartChild(const int32 I){};

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UBBase>> Children;
};
