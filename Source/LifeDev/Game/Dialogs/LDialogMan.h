// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Dialogs/DialogManager.h"

#include "LDialogMan.generated.h"

// base class for the character
UCLASS(Blueprintable, config=Game)
class LIFEDEV_API ALDialogMan : public ADialogManager {
	GENERATED_BODY()

public:
	
	ALDialogMan();
};