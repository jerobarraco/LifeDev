// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Books.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooks: public ALInteract {
	GENERATED_BODY()

public:
	ABooks();
	ABooks(int32 nBookCount, int32 nRndSeed);
	
protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<UCQuickMesh*> Books;
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
	int32 RndSeed = 4;
	bool RndFull = true;
	float RndOff = 2;
	float Spacing = 2.1;

private:
	void SetUpInteract() const;
	void CreateBooks();
	void Constructor();
};
