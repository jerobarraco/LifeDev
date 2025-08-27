// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"

#define SoftOP(Class, Path) TSoftObjectPtr<Class>(FSoftObjectPath(Path))