// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LSave.generated.h"

// VERY IMPORTANT NOTE:
// DO NOT READ/WRITE TO THIS CLASS DYNAMICALLY DURING GAMEPLAY
// THIS CLASS IS SUPPOSED TO STORE JUST A SNAPSHOT AND THEN USE READ/WRITESUBSYSTEMS.
// baseclass for lifedev savegame
// this class can't access the world
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSave : public USaveGame {
	GENERATED_BODY()

public:
	// resets the savestate. it will write to subsystems.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	void Reset(const UObject* const O);

	// sets the subsystem data to what's on the savestate.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	void WriteSubsystems(const UObject* const O);

	// sets the savestate data to what's on the subsystems.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	void ReadSubsystems(const UObject* O, const bool WithInventory = true);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 ItemsNum() const { return SInventory.Num(); }

	// this is just for internal use.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(AdvancedDisplay))
	FORCEINLINE TMap<FName, float> GetFlags() { return SFlags; } // can't use Const on tmaps :'(

	// the current chapter.
	// it will save the chapterid past the last.
	// why? because that way i can differentiate someone who's playing the last chapter and someone that finished the game.
	// Settings->GetChapterFeat will return C_DONE
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChapterID = -1;

	// last save time
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 Time = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Version = -1;

	static const inline int32 VersionLast = 1;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, float> SFlags;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, int32> SInventory;
	// saved feats
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSet<EFeat> SFeats;

	// the features to affect during load/save, the rest will be ignored.
	inline static TSet<EFeat> WatchFeats = {
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_AUTO_EXP, EFeat::V_FRINGE, EFeat::V_FOV, EFeat::V_BLUR, EFeat::V_DUST,
		EFeat::V_LUMEN, EFeat::V_MLIGHTS, EFeat::V_NANITE, EFeat::V_SHAD_RAY,
		EFeat::V_STROBE, EFeat::V_FLASHBACK,
		 // EFeat::V_SPEED
		EFeat::U_TICK_BATCH, EFeat::U_TICK_BATCH,

		EFeat::D_AUTO, EFeat::D_TEXT,
		
		EFeat::G_CARD0, EFeat::G_CARD1, EFeat::G_CARD2, EFeat::G_CARD3, EFeat::G_CARD4,
		EFeat::G_GHOSTS, EFeat::G_TEACH, EFeat::G_STATUS, EFeat::G_RUMBLE, EFeat::G_FB_ANIM,
		EFeat::G_SHOW_POINT
	};
	// EFeat::D_SHOW, // since this is not on the previous savegames,
	// and also is not on the settings in user builds, it's better not to watch it,
	// or it will default to false.
	// TODO implement versioning
	// todo rename watch to something else, maybe
};
