
#pragma once
#include "InvItemsNames.generated.h"

// This is tentative, is it good or just bloating things up? 
USTRUCT(BlueprintType, Blueprintable)
struct FItemNames {
	GENERATED_BODY()
public:
	inline static FName Card0 = FName("C0");
	inline static FName Poem0 = FName("P00");
};
