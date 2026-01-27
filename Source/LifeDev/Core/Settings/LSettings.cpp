// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LSettings.h"

#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

#include "JUtils/Misc/JUtilsMisc.h"

#include "LSave.h"
#include "LSysSettings.h"
#include "Interact/Interact.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSettings, Log, Log);

static const FString SaveSlot("LifeDev_");

ULSettings* ULSettings::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const UGameInstance* const Instance = World->GetGameInstance();
	if (UNLIKELY(!IsValid(Instance))) return nullptr;
	
	return Instance->GetSubsystem<ULSettings>();
}

void ULSettings::NewGame(const int32 NewSlotIndex) {
	if (UNLIKELY(IsSaving)) {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Aborted. Save system is busy. Stop."), __func__);
		// not triggering OnSaving here since we're just aborting, and something else must be in the works.
		return;
	}

	SetIsSaving(true);
	// Instantiate a new SaveGame object
	Save = Cast<ULSave>(UGameplayStatics::CreateSaveGameObject(ULSave::StaticClass()));
	if (UNLIKELY(!Save)) {
		UE_LOG(LogLSettings, Warning,
			TEXT("%hs: Failed to create a new savegame. Can't progress."), __func__);
		return;
	}

	SlotIndex = NewSlotIndex;
	UE_LOG(LogLSettings, Log, TEXT("%hs: Slot=%i."), __func__, SlotIndex);
	Save->Reset(this); // does write subsystem
	SetIsSaving(false);  // broadcast anyway since the game mode will be waiting.
}

void ULSettings::SetIsSaving(const bool NewIsSaving) {
	IsSaving = NewIsSaving;
	OnSaving.Broadcast(NewIsSaving);
}

void ULSettings::LoadGame(const int32 NewSlotIndex) {
	// check before modifying internal state
	if (UNLIKELY(IsSaving)) {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Load game aborted, save system is busy. STOP"), __func__);
		// not broadcasting here, since something else is working.
		return;
	}

	SetIsSaving(true);

	// update target slot
	if (NewSlotIndex>=0) SlotIndex = NewSlotIndex;
	if (UNLIKELY(SlotIndex<0)) SlotIndex = 0; // ensure a valid slot index

	const FString& SlotName = SaveSlot + FString::FromInt(SlotIndex);
	UE_LOG(LogLSettings, Log, TEXT("%hs. SlotName=%s"), __func__, *SlotName);

	FAsyncLoadGameFromSlotDelegate OnLoadGameDone;
	OnLoadGameDone.BindUObject(this, &ULSettings::LoadGameDone);
	// Try to load a saved game file (with name: <SaveSlot>.sav) if exists
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, OnLoadGameDone);
}

void ULSettings::SaveGame(const int32 NewSlotIndex, const bool WithInventory) {
	if (UNLIKELY(IsSaving)) {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Save game aborted, save system is busy. Stop."), __func__);
		// not triggering OnSaving here since we're just aborting, and something else must be in the works.
		return;
	}

	if (UNLIKELY(!Save)) {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Save game aborted. No savegame to save. Stop"), __func__);
		SetIsSaving(false); // technically done. important or objects might get stuck (gamemode)
		return;
	}

	SetIsSaving(true);

	// update slot index. If parameter is set use that.
	if (NewSlotIndex>=0) SlotIndex = NewSlotIndex;

	// if it's invalid force to 0
	if (SlotIndex < 0) SlotIndex = 0;

	const FString& SlotName = SaveSlot + FString::FromInt(SlotIndex);
	UE_LOG(LogLSettings, Log, TEXT("%hs. SlotName='%s' WithInventory=%i "), __func__,
		*SlotName, WithInventory);

	Save->ReadSubsystems(this, WithInventory);

	// if savegame is disabled. don't actually save to disk. still keep the savegame working "as if" in memory.
	// so that other object can also work "as if".
	if (UNLIKELY(!GetFeat(EFeat::G_SAVE))) {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Save game aborted. Savegame feature flag is unset. Skip"), __func__);
		// technically done. important or objects might get stuck (gamemode)
		SaveGameDone(SlotName, 0, true); 
		return;
	}

	FAsyncSaveGameToSlotDelegate OnSaveGameDone;
	OnSaveGameDone.BindUObject(this, &ULSettings::SaveGameDone);
	UGameplayStatics::AsyncSaveGameToSlot(Save, SlotName, 0, OnSaveGameDone);
}

void ULSettings::SaveGameDone(const FString& Slot, const int32 Index, const bool Success) {
	// Call SaveGameToSlot to serialize and save our SaveGameObject with name: <SaveGameSlotName>.sav
	if (LIKELY(Success)) {
		UE_LOG(LogLSettings, Log, TEXT("%hs Savegame saved"), __func__);
	} else {
		UE_LOG(LogLSettings, Warning, TEXT("%hs Savegame save failed."), __func__);
	}

	// trigger on both cases. important or objects might get stuck.
	SetIsSaving(false);
}

void ULSettings::LoadGameDone(const FString& Slot, const int32 Index, USaveGame* const LoadedGame) {
	Save = Cast<ULSave>(LoadedGame);
	if (UNLIKELY(!Save)) {
		// If file does not exist, try to create a new one
		UE_LOG(LogLSettings, Log, TEXT("%hs No savefile found, creating a new one. Slot=%i"),
			__func__, SlotIndex);
		// should assign the slot index here.
		// otherwise if a game load fails for a given slot. it will override slot 0.
		// that'd be terrible!
		IsSaving = false;// clear only for call to new game. internal. no broadcast.
		NewGame(SlotIndex); // broadcasts inside. important.
		return;
	}

	if (UNLIKELY(Save->Version != Save->VersionLast)) {
		UE_LOG(LogLSettings, Log, TEXT("%hs Version does not match!. Slot=%i VersionLast=%i Version=%i"),
			__func__, SlotIndex, Save->VersionLast, Save->Version);
		// TODO "do something!"
	}

	// i've decided i don't need to do this here. it brings a lot of problems.
	// it's better to write subsystems manually.
	// Save->WriteSubsystems(GetWorld());

	UE_LOG(LogLSettings, Log, TEXT("%hs Load game succeeded."), __func__);
	SetIsSaving(false);
}

int32 ULSettings::CurrentChapter() const {
	return LIKELY(IsValid(Save)) ? Save->ChapterID : -1;
}

EFeat ULSettings::CurrentChapterFeat() const {
	const int32 ChId = CurrentChapter();
	constexpr int32 Max = UJUtilsMisc::ArraySize(LDConsts::Feats::ChapFeats);
	if (UNLIKELY(ChId < 0)) return EFeat::NONE;

	// note that this feat is not saved in the savegame. since i want to support adding more chapters later.
	if (UNLIKELY(ChId >= Max)) return EFeat::C_DONE;

	return LDConsts::Feats::ChapFeats[ChId];
}

void ULSettings::ResetFeats() {
	const ULSysSettings* const Settings = ULSysSettings::Get();
	if (UNLIKELY(!Settings)) return;

	Feats = Settings->GetFeats();
}

void ULSettings::SetFeat(const EFeat Feat, const bool Enable) {
	const bool Has = Feats.Contains(Feat);
	const bool Changed = Enable != Has;
	if (Enable) {
		if (!Has) Feats.Add(Feat);
	} else {
		if (Has) Feats.Remove(Feat);
	}

	if (UNLIKELY(!Changed)) return;

	FeatUpdated(Feat, Enable);
}

bool ULSettings::GetFeatS(const UObject* const O, const EFeat Feat) {
	const ULSettings* const I = ULSettings::Instance(O);
	return LIKELY(IsValid(I)) ? I->GetFeat(Feat) : false;
}

FName ULSettings::GetObjectLabel(const UObject* const Object) {
	const AInteract* const Inter = Cast<AInteract>(Object);
	if (LIKELY(Inter)) return Inter->GetLabel();

	return Inter->GetFName();
}

void ULSettings::FeatsDump() const {
	// Not sure if i should surround this with shipping only. not for now.
	// even if logs are disabled by default on shipping, i might need it some day.
	// and the player can't destroy anything with this.

	for (const EFeat F: Feats) {
		UE_LOG(LogLSettings, Log, TEXT("%hs: %s"), __func__, *UEnum::GetValueAsString(F));
	}
}

void ULSettings::Init() {
	ResetFeats(); // important to do before checking for savegame, or the getFeat below won't work.
	// also i need it for the feats that are not watched.
	
	// clear. and force for loadgame. not broadcasting here since it will confuse the caller, this is internal only.
	IsSaving = false;

	// very important that all paths broadcasts. since the game mode will wait for it.
	// notice call to ResetFeats above, we can simply query here.
	// likely since we favor shipping, and it SHOULD(TM) be enabled on shipping.
	if (LIKELY(GetFeat(EFeat::G_SAVE))) LoadGame();
	else NewGame();
}

void ULSettings::FeatUpdated(const EFeat Feat, const bool Enable) const {
	OnFeatUpdate.Broadcast(Feat, Enable);

	if ((Feat >= EFeat::C_00) & (Feat < EFeat::C_MAX))
		OnFeatUpdateChap.Broadcast(Feat, Enable);
	else if((Feat >= EFeat::D_SHOW) & (Feat < EFeat::D_MAX))
		OnFeatUpdateDiags.Broadcast(Feat, Enable);
	else if((Feat >= EFeat::S_MUSIC) & (Feat < EFeat::S_MAX))
		OnFeatUpdateSound.Broadcast(Feat, Enable);
	else if((Feat >= EFeat::V_LUMEN) & (Feat < EFeat::V_MAX)) // < since not including the max)
		OnFeatUpdateVisual.Broadcast(Feat, Enable);
	else if((Feat >= EFeat::G_CARD0) & (Feat < EFeat::G_MAX))
		OnFeatUpdateGameplay.Broadcast(Feat, Enable);
	else if((Feat >= EFeat::DBG_STEPS) & (Feat < EFeat::DBG_MAX))
		OnFeatUpdateDebug.Broadcast(Feat, Enable);
	else if((Feat>=EFeat::U_TICK_BATCH) & (Feat < EFeat::U_MAX))
		OnFeatUpdateUnreal.Broadcast(Feat, Enable);
}
