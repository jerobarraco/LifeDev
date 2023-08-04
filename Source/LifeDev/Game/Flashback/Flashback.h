// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "Flashback.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnChange, float, Value);

// Base subsystem for flashbacks
UCLASS(Blueprintable, Category="LifeDev")
class LIFEDEV_API UFlashback : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFlashback();

	UFUNCTION(BlueprintCallable)
	float GetVal() const { return Val; }

	UFUNCTION(BlueprintCallable)
	void SetVal(float New);

	UFUNCTION(BlueprintCallable)
	void IncVal(float By);
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FFBOnChange OnChange;

protected:
	float Val = 0;
};
