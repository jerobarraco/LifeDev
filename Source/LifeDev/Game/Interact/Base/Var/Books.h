// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Books.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooks: public ALInteract {
	GENERATED_BODY()

public:
	ABooks();
	ABooks(const int32 nBookCount, const int32 nRndSeed =0);

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	virtual void Constructor();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TObjectPtr<UCQuickMesh>> Books;
	// metas are there to be able to view them on the editor

	UPROPERTY()
	TArray<TSoftObjectPtr<UMaterialInterface>> Materials = {
		TSoftObjectPtr<UMaterialInterface>(
			FSoftObjectPath("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI.Palette00_DMI")),
		TSoftObjectPtr<UMaterialInterface>(
			FSoftObjectPath("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette01_DMI.Palette01_DMI")),
		TSoftObjectPtr<UMaterialInterface>(
			FSoftObjectPath("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette02_DMI.Palette02_DMI")),
		TSoftObjectPtr<UMaterialInterface>(
			FSoftObjectPath("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette03_DMI.Palette03_DMI")),
	};

	int32 BookCount = 5;
	// if ==0 then it will be random on constructor.
	// otherwise it will be static to the value (including negatives)
	int32 RndSeed = 0;
	float RndOff = 2;
	float Spacing = 2.1;

private:
	void SetUpInteract() const;
	void CreateBooks();
};
