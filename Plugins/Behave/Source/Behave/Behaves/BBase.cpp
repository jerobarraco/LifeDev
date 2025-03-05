// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "BBase.h"

void UBBase::Tick_Implementation(float DT) {}
float UBBase::Has_Implementation(const FName& Token) { return 0; }
float UBBase::Want_Implementation(const FName& Token) { return 0; }
float UBBase::Need_Implementation(const FName& Token) { return 0; }
void UBBase::Do_Implementation(const FName& Token) {}
