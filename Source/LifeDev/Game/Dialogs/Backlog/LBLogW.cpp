// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LBLogW.h"

#include "Diags/Diags.h"

void ULBLogW::NativeOnInitialized() {
	Super::NativeOnInitialized();
	UDiags* const Diags = UDiags::Instance(this);
	if (UNLIKELY(!Diags)) return;
	Diags->OnAdd.AddUniqueDynamic(this, &ULBLogW::AddDiag);
	// bind to the diags
}

void ULBLogW::NativeDestruct() {
	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags))
		Diags->OnAdd.RemoveDynamic(this, &ULBLogW::AddDiag);

	Super::NativeDestruct();
}

void ULBLogW::AddDiag(const FName Name, const FDialog& Diag) {
	// TODO
	Seen.Add(Name);
}
