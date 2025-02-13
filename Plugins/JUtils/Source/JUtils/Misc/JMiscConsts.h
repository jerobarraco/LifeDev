// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#define IFL(x) if(LIKELY(x))
#define IFU(x) if(UNLIKELY(x))

// these are experimental
#define IFC(x, exp) if(LIKELY(x)) x->exp;
#define IFVC(x, exp) if(LIKELY(IsValid(x))) x->exp;
