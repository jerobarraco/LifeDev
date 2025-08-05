// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "BooksB.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABooksB: public ALInteract {
	GENERATED_BODY()

public:
	ABooksB();

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Books")
	void ReCreate();
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Books")
	void Randomize() { RndSeed = FMath::Rand(); ReCreate(); }

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	int32 BookCount = 5;
	// if ==0 then it will be random on constructor.
	// otherwise it will be static to the value (including negatives)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	int32 RndSeed = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	float RndOff = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	float Spacing = 2.1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	TObjectPtr<UStaticMesh> BaseMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Books")
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TArray<TObjectPtr<UCQuickMesh>> Books;
	// metas are there to be able to view them on the editor

private:
	void SetUpInteract() const;
	void DestroyBooks();
	void CreateBooks();
};
