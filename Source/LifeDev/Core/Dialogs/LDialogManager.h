// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Dialogs/DialogManager.h"
#include "LDialogManager.generated.h"

class UDialogUI;

// base class for the character
UCLASS(Blueprintable, config=Game)
class DIALOGS_API ALDialogManager : public ADialogManager {
	GENERATED_BODY()

public:
	
	ALDialogManager();
};