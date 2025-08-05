// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "BooksB.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooksB: public ALInteract {
	GENERATED_BODY()

public:
	ABooksB();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ReCreate();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Randomize() { RndSeed = FMath::Rand(); ReCreate(); }

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 BookCount = 5;
	// if ==0 then it will be random on constructor.
	// otherwise it will be static to the value (including negatives)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 RndSeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float RndOff = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Spacing = 2.1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TObjectPtr<UStaticMesh> BaseMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TArray<TObjectPtr<UCQuickMesh>> Books;
	// metas are there to be able to view them on the editor

private:
	void SetUpInteract() const;
	void DestroyBooks();
	void CreateBooks();
};
