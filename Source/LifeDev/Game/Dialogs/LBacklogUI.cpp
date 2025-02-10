// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LBacklogUI.h"

#include "Diags/Diags.h"

void ULBacklogUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	UDiags* const Diags = UDiags::Instance(this);
	if (UNLIKELY(!Diags)) return;
	Diags->OnAdd.AddUniqueDynamic(this, &ULBacklogUI::AddDiag);
	// bind to the diags
}

void ULBacklogUI::NativeDestruct() {
	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags))
		Diags->OnAdd.RemoveDynamic(this, &ULBacklogUI::AddDiag);

	Super::NativeDestruct();
}

void ULBacklogUI::AddDiag(const FName Name, const FDialog& Diag) {
	// TODO
	Seen.Add(Name);
}
