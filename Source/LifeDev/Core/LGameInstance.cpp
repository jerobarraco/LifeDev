// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LGameInstance.h"


/*

void USGEGameInstance::LoadGame()
{
	// Try to load a saved game file (with name: <SaveGameSlotName>.sav) if exists
	USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	SaveGameObject = Cast<USGESaveGame>(LoadedGame);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Trying to load a saved game."));
    
	// If file does not exist try create a new one
	if (!SaveGameObject)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No saved games found. Trying to save a new one."));
        
		// Instantiate a new SaveGame object
		SaveGameObject = Cast<USGESaveGame>(UGameplayStatics::CreateSaveGameObject(USGESaveGame::StaticClass()));

		// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
		const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

		LogIfGameWasSavedOrNot(IsSaved);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saved game found. Loaded."));
	}
    
}

SaveGame method:

void USGEGameInstance::SaveGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Saving game..."));
    
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	const bool IsSaved = UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);

	LogIfGameWasSavedOrNot(IsSaved);
    
}
*/