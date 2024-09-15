// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LSettings.h"

#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "LSave.h"
#include "LSysSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSettings, Log, Log);

static const FString SaveSlot("LifeDev_");

ULSettings* ULSettings::Instance(UObject* O) {
	if (!IsValid(O)) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (!IsValid(World)) return nullptr;

	const UGameInstance* const Instance = World->GetGameInstance();
	if (!IsValid(Instance)) return nullptr;
	
	return Instance->GetSubsystem<ULSettings>();
}

void ULSettings::NewGame(int32 NewSlotIndex) {
	// Instantiate a new SaveGame object
	Save = Cast<ULSave>(UGameplayStatics::CreateSaveGameObject(ULSave::StaticClass()));
	if (!Save) {
		UE_LOG(LogLSettings, Warning,
			TEXT("%hs: Failed to create a new savegame. Can't progress."), __func__);
		return;
	}

	SlotIndex = NewSlotIndex;
	UE_LOG(LogLSettings, Log, TEXT("%hs: Slot=%i."), __func__, SlotIndex);
	Save->Reset(GetWorld()); // does write subsystem
}

void ULSettings::LoadGame(int32 NewSlotIndex) {
	// check before modifying internal state
	if (IsSaving) {
		UE_LOG(LogLSettings, Warning, TEXT("Load game aborted, save system is busy."));
		return;
	}
	IsSaving = true;

	// update target slot
    if (NewSlotIndex>=0) SlotIndex = NewSlotIndex;
	if (SlotIndex<0) SlotIndex = 0;

	const FString& SlotName = SaveSlot + FString::FromInt(SlotIndex);
	UE_LOG(LogLSettings, Log, TEXT("%hs. SlotName=%s"), __func__, *SlotName);

	FAsyncLoadGameFromSlotDelegate OnLoadGameDone;
	OnLoadGameDone.BindUObject(this, &ULSettings::LoadGameDone);
	// Try to load a saved game file (with name: <SaveSlot>.sav) if exists
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, OnLoadGameDone);
}

void ULSettings::SaveGame(int32 NewSlotIndex) {
	// TODO should i skip saving a game if UseSaveGame is false in LSysSettings????
	// -- prolly not. since i still need to test the savegame functionality during gameplay
	
	if (!Save) {
		UE_LOG(LogLSettings, Warning, TEXT("Save game aborted. No savegame to save."));
		OnSaveReady.Broadcast(); // technically done. important or objects might get stuck
		return;
	}
	
	if (IsSaving) {
		UE_LOG(LogLSettings, Warning, TEXT("Save game aborted, save system is busy."));
		// not triggering onSaveReady here since there must be something else in queue.
		return;
	}
	IsSaving = true;

	// update slot index. If parameter is set use that.
	if (NewSlotIndex>=0) SlotIndex = NewSlotIndex;

	// if it's invalid force to 0
	if (SlotIndex < 0) SlotIndex = 0;

	const FString& SlotName = SaveSlot + FString::FromInt(SlotIndex);
	UE_LOG(LogLSettings, Log, TEXT("%hs. SlotName=%s"), __func__, *SlotName);

	// TODO add time here. how?
	Save->ReadSubsystems(GetWorld());
	
	FAsyncSaveGameToSlotDelegate OnSaveGameDone;
	OnSaveGameDone.BindUObject(this, &ULSettings::SaveGameDone);
	UGameplayStatics::AsyncSaveGameToSlot(Save, SlotName, 0, OnSaveGameDone);
}

void ULSettings::SaveGameDone(const FString& Slot, int32 Index, bool Success) {
	IsSaving = false;
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	if (Success) {
		UE_LOG(LogLSettings, Log, TEXT("Savegame saved"));
	} else {
		UE_LOG(LogLSettings, Warning, TEXT("Savegame save failed."));
	}

	// trigger on both cases. important or objects might get stuck.
	OnSaveReady.Broadcast();
}

void ULSettings::LoadGameDone(const FString& Slot, int32 Index, USaveGame* LoadedGame) {
	IsSaving = false;
	Save = Cast<ULSave>(LoadedGame);
	if (!Save) {
		// If file does not exist, try to create a new one
		UE_LOG(LogLSettings, Log, TEXT("No savefile found, creating a new one. Slot=%i"), SlotIndex);
		// should assign the slot index here.
		// otherwise if a game load fails for a given slot. it will override slot 0.
		// that'd be terrible!
		NewGame(SlotIndex);
		OnSaveReady.Broadcast(); // broadcast anyway since someone might be waiting on this.
		return;
	}

	// TODO should i really do this here?
	Save->WriteSubsystems(GetWorld());
	
	UE_LOG(LogLSettings, Log, TEXT("Load game succeeded."));
	OnSaveReady.Broadcast();
}

int32 ULSettings::CurrentChapter() const {
	return IsValid(Save) ? Save->ChapterID : -1;
}

void ULSettings::ResetFeats() {
	ULSysSettings* const Settings = ULSysSettings::Get();
	if (!Settings) return;
	Feats = Settings->GetFeats();
}

void ULSettings::SetFeat(EFeat Feat, bool Enable) {
	const bool Has = Feats.Contains(Feat);
	const bool Changed = Enable != Has;
	if (Enable) {
		if (!Has) Feats.Add(Feat);
	} else {
		if (Has) Feats.Remove(Feat);
	}

	if (!Changed) return;
	FeatUpdated(Feat, Enable);
}

bool ULSettings::GetFeatS(UObject* const O, const EFeat Feat) {
	const ULSettings* const I = ULSettings::Instance(O);
	return IsValid(I) ? I->GetFeat(Feat) : false;
}

void ULSettings::Init() {
	ResetFeats();
	IsSaving = false;

	const ULSysSettings* const Settings = ULSysSettings::Get();
	if (Settings && Settings->UseSaveGame) LoadGame();
	else NewGame();
}

void ULSettings::FeatUpdated(const EFeat Feat, const bool Enable) const {
	OnFeatUpdate.Broadcast(Feat, Enable);

	if (Feat >= EFeat::C_00 && Feat <= EFeat::C_09)
		OnFeatUpdateChap.Broadcast(Feat, Enable);
	else if (Feat >= EFeat::D_ALL && Feat <= EFeat::D_TEXT)
		OnFeatUpdateDiags.Broadcast(Feat, Enable);
	else if (Feat >= EFeat::S_MUSIC && Feat <= EFeat::S_ENV)
		OnFeatUpdateSound.Broadcast(Feat, Enable);
	else if (Feat >= EFeat::V_LUMEN && Feat <= EFeat::V_SPEED)
		OnFeatUpdateVisual.Broadcast(Feat, Enable);
	else if (Feat == EFeat::E_GHOSTPOOL)
		OnFeatUpdateSound.Broadcast(Feat, Enable); // this is just a patch for the lmusicman.
	// else if (Feat >= EFeat::V_STROBE && Feat <= EFeat::V_FOV) // accessibility yet.
		// OnFeatUpdateAccess.Broadcast(Feat, Enable);
	else if (Feat >= EFeat::DBG_STEPS && Feat <= EFeat::DBG_ALL)
		OnFeatUpdateDebug.Broadcast(Feat, Enable);
	
}
