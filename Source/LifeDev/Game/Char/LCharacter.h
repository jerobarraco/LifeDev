// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inventory/Inventory.h"

#include "LCharacter.generated.h"

class UCInteractor;
class UCInteract;
class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGameUI;

// base class for the character
UCLASS(config=Game)
class LIFEDEV_API ALCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALCharacter();
	
	UFUNCTION(BlueprintCallable)
	void SetUIVisible(bool bCond);
	
	UFUNCTION(BlueprintCallable)
	void SetInputEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void InteractSetEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void LookItem(const FItem& Item);

	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Name);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UGameUI> UIClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputMappingContext* Mapping = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionJump = nullptr;

	//* Move Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* ActionMove = nullptr;
		
	//* Look Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* ActionLook = nullptr;

	//* Interact Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* ActionInteract = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* ActionItem = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* ActionItemLook = nullptr;

protected:
	UFUNCTION()
	void InteractBegin(UCInteract* Comp);
	UFUNCTION()
	void InteractEnd(UCInteract* Comp);

	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//* Called for movement input 
	void ActMove(const FInputActionValue& Value);
	//* Called for looking input 
	void ActLook(const FInputActionValue& Value);
	void ActInteract(const FInputActionValue& Value);
	void ActItem();
	void ActItemLook();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	//* Pawn mesh: 1st person view (arms; seen only by self) 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	// First person camera
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCInteractor* Interactor = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	UGameUI* UI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
};

