#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

template <typename T>
static void GetAllTableRows(const UDataTable* Table, TArray<T>& OutRows)
{
	OutRows.Empty();
	
	if (!IsValid(Table))
	{
		UE_LOG(LogTemp, Error, TEXT("Data Table is invalid."));
		return;
	}
	
	TArray<T*> TmpRows;
	// done this way to avoid adding null rows and because you can't expose an array of pointers to blueprints
	// (which is what getallrows has)
	Table->GetAllRows<T>(TEXT(""), TmpRows);
	for (const T* Row: TmpRows)
	{
		if (!Row) continue;
		OutRows.Add(*Row);
	}
}


static bool FStringLooseEquals(const FString& A, const FString& B)
{
	// Receives a copy since we will modify them. But using both inlines will be faster than calling Trim().Lower().
	return A.TrimStartAndEnd().Equals(
		B.TrimStartAndEnd(),
		ESearchCase::IgnoreCase
	);
}
 

static bool IsEditor() 
{
	#if WITH_EDITOR
		return true;
	#else
		return false;
	#endif
}