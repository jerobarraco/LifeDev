// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "../BTypes.h"

#include "BBase.generated.h"

// base for action behaviors. the idea is that you make your own and each one handles what it needs.
// a top level action, is considered a goal.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UObject {
	GENERATED_BODY()

public:
	void SetState(int32 TODO) {};
	int32 GetState(){return -1;};
	
	float Cost() {return 0;};
	float CostSelf() {return 0;};

	// called from a bg thread.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	// this considers itself first, THEN the children.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanDo() const;

	// called from a bg thread.
	// this should only consider itself. and not the children.
	// needs to be as fast as possible but don't worry if it's a bit slow.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanDoSelf() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EBDoRes Do(const float DT);

	void* OnState;

protected:
	void StartChild(const int32 I){};

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UBBase>> Children;
};
