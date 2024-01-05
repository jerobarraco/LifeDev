// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Diags/DiagMan.h"

#include "LDialogMan.generated.h"

// base class for the character
UCLASS(Blueprintable)
class LIFEDEV_API ALDialogMan : public ADiagMan {
	GENERATED_BODY()

public:
	ALDialogMan();
};