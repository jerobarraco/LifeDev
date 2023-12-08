// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LSettings.h"

#include "LSave.h"
#include "LSysSettings.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSettings, Log, Log);

static const FString SaveSlot("LifeDev");

ULSettings* ULSettings::Get(UWorld* World) {
	if (!IsValid(World))  return nullptr;
	UGameInstance* const Instance = World->GetGameInstance();
	if (!IsValid(Instance))  return nullptr;
	
	return Instance->GetSubsystem<ULSettings>();
}

void ULSettings::NewGame() {
	// Instantiate a new SaveGame object
	Save = Cast<ULSave>(UGameplayStatics::CreateSaveGameObject(ULSave::StaticClass()));
	Save->Reset();

	SaveGame();
}

void ULSettings::LoadGame() {
	UE_LOG(LogLSettings, Log, TEXT(" %hs"), __func__);
	// TODO use the slot at some point
	// Try to load a saved game file (with name: <SaveSlot>.sav) if exists
	USaveGame* const LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveSlot, 0);
	Save = Cast<ULSave>(LoadedGame);

	if (Save) {
		UE_LOG(LogLSettings, Log, TEXT("Success loading"));
		return;
	}

	// If file does not exist try create a new one
	UE_LOG(LogLSettings, Log, TEXT("No savefile found, creating a new one"));
	NewGame();
}

void ULSettings::SaveGame() {
	UE_LOG(LogLSettings, Log, TEXT("%hs"), __func__);
    
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	const bool IsSaved = UGameplayStatics::SaveGameToSlot(Save, SaveSlot, 0);
	if (IsSaved) {
		UE_LOG(LogLSettings, Log, TEXT("Savegame saved"));
	} else {
		UE_LOG(LogLSettings, Warning, TEXT("Savegame save failed."));
	}
}

bool ULSettings::IsDebugBuild() {
#if (UE_BUILD_TEST || UE_BUILD_SHIPPING)
	return true;
#else
	return false;
#endif
}

void ULSettings::ResetFeats() {
	ULSysSettings* const Settings = ULSysSettings::Get();
	const bool UseDebug = ULSettings::IsDebugBuild() && Settings->UseDebugFeats;
	Feats = UseDebug ? Settings->DebugFeats : Settings->DefaultFeats;
}

void ULSettings::SetFeat(EFeat Feat, bool Enable) {
	const bool Has = Feats.Contains(Feat);
	if (Enable) {
		if (!Has) Feats.Add(Feat);
	} else {
		if (Has) Feats.Remove(Feat);
	}
	
	OnFeatUpdate.Execute(Feat, Enable);
}

bool ULSettings::GetFeatS(UWorld* World, EFeat Feat) {
	ULSettings* const I = ULSettings::Get(World);
	return IsValid(I)? I->GetFeat(Feat) : false;
}

void ULSettings::Init() {
	LoadGame();
	ResetFeats();
}
