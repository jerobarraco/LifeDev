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

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<TObjectPtr<UCQuickMesh>> Books;
	// metas are there to be able to view them on the editor
	
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

private:
	void SetUpInteract() const;
	void DestroyBooks();
	void CreateBooks();
};
