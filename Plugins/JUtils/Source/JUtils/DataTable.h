// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

template <typename T>
static bool ReadTable(const UDataTable* DT, TArray<T>& OutRows)
{
	OutRows.Empty();
	if (!IsValid(DT)) {
		UE_LOG(LogTemp, Error, TEXT("Data Table is not valid or unassigned."));
		return false;
	}
	
	TArray<T*> RawRows;
	// Can't pass pointers to bps, and don't want null values either
	Table->GetAllRows<T>(TEXT(""), RawRows);
	const int32 Count = RawRows.Num();
	for (int32 i= 0; i< Count; ++i)
	{
		const T* R = RawRows[i];
		if (!R) continue;
		OutRows.Add(*R);
	}

	return true;
}
