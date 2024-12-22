#pragma once

#include "CoreMinimal.h"
#include "JUtilsEditor.generated.h"

// DECLARE_DYNAMIC_DELEGATE_OneParam(FJUEOnPie, const bool, IsSimulating);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJUEOnPieD, const bool, IsSimulating);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsEditor: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
// 	virtual void PostLoad() override;
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "JUtils Editor")
// 	FJUEOnPie OnPie;
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "JUtils Editor")
// 	FJUEOnPieD OnPieD;
//
// 	UFUNCTION(BlueprintCallable)
// 	static void SetOnPie(const FJUEOnPie& NOnPie);
//
// protected:
// 	UFUNCTION()
// 	void BeginPie(const bool IsSimulating) const {
// 		OnPie.ExecuteIfBound(IsSimulating);
// 		OnPieD.Broadcast(IsSimulating);
// 	};
};

