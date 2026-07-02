// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "JUtilsNet.generated.h"


// These macros are still function calls, cache them if possible.
// notice that you can infer some from others.
// https://forums.unrealengine.com/t/hasauthority-returning-true-for-client/54394/9

/// simple ones

// client (not stand, dedicated nor listen)
#define JU_IsClient IsNetMode(NM_Client)
// this will include Standalone, Dedicated and Listen server
#define JU_IsServerSide (!JU_IsClient)
// when needed to differentiate server from standalone
#define JU_IsStandalone IsNetMode(NM_Standalone)

/// complex

// used for things that should happen only on the client, or when in standalone.
// e.g. some ui.
#define JU_IsClientSide (JU_IsStandalone || JU_IsClient)
// for things that should ONLY happen on connected servers (dedicated and listen clients)
// not on client nor standalone
// e.g. storing or not some objects.
#define JU_IsServerOnly (!JU_IsClientSide) // SU_IsServer && !SU_IsStandalone)

UCLASS(Blueprintable)
class JUTILS_API UJUtilsNet: public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	
};
