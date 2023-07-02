// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Muse.h"

#include "OSC/Public/OSCServer.h"

#pragma optimize("", off)

UMuse::UMuse():Super() {
}

void UMuse::Init_Implementation(const FString& Address, int32 Port) {
	Server = NewObject<UOSCServer>();
	if (!IsValid(Server)) return;
	
	Server->SetAddress(Address, Port);
	Server->Listen();
}

void UMuse::DeInit_Implementation() {
	if (!IsValid(Server)) return;
}

#pragma optimize("", on)
