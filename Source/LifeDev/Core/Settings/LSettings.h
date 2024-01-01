// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LSysSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSettings.generated.h"

class ULSave;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeatUpdate, EFeat, Feat, bool, Enabled);

class ULGUSettings;

// This class allows to interact with the game settings during gameplay.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettings : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static ULSettings* Get(UWorld* World);
	UFUNCTION(BlueprintCallable)
	static bool GetFeatS(UWorld* World, EFeat Feat);

	UFUNCTION(BlueprintCallable)
	void NewGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CurrentChapter() const;
	
	UFUNCTION(BlueprintCallable)
	void ResetFeats();
	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat, bool Enable);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	bool GetFeat(EFeat Feat) const {
		return Feats.Contains(Feat);
	}
	
	// only call once by the gameinstance or smth
	UFUNCTION(BlueprintCallable)
	void Init();

	UPROPERTY(BlueprintReadOnly, Transient)
    ULSave* Save = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdate;
	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdateChap;
	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdateAccess;
	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdateSound;
	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdateGraph;
	UPROPERTY(BlueprintReadWrite, Transient)
	FOnFeatUpdate OnFeatUpdateMisc;


protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	TSet<EFeat> Feats;
};
