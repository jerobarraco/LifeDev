// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "JUtilsEditor.generated.h"

// DECLARE_DYNAMIC_DELEGATE_OneParam(FJUEOnPie, const bool, IsSimulating);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJUEOnPieD, const bool, IsSimulating);

UCLASS(Blueprintable)
class JUTILS_API UJUtilsEditor: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
	// adds a section to the outliner properties. calling this multiple times will add the categories
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Class,Section, Categories"))
	static bool AddOutlinerSection(const FString& Class, const FString& Section, const TArray<FString>& Categories);
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(AutoCreateRefTerm="Class,Section, Categories"))
	static bool PlayInEditor();

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

