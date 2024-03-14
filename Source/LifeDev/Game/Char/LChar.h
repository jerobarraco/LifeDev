// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inventory/Inventory.h"

#include "LChar.generated.h"

class ULSettingsUI;
class UDiags;
class UCInteractor;
class UCInteract;
class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGameUI;
class UCLNoiser;

// TODO consider moving the input stuff to another component
// TODO consider moving the item interaction dialog stuff to another component
// TODO consider moving the settings to another component

// base class for the character
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALChar : public ACharacter {
	GENERATED_BODY()

public:
	ALChar();
	
	UFUNCTION(BlueprintCallable)
	void SetUIVisible(bool bCond);

	// fully disables the input of the character. use InteractSetEnabled instead unless you really need to.
	UFUNCTION(BlueprintCallable)
	void SetInputEnabled(bool Enabled);

	// disable the interact for the character.
	UFUNCTION(BlueprintCallable)
	void InteractSetEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void LookItem(const FName& Name);

	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Name);

	// factor to apply to look when hovering an interact
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float InteractDrag = .3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float SpeedMin = 65;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float SpeedMax = 150;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UGameUI> UIClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<ULSettingsUI> SettingsUIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UInputMappingContext* Mapping = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UInputAction* ActionJump = nullptr;

	//* Move Input Action 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	UInputAction* ActionMove = nullptr;
		
	//* Look Input Action 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	UInputAction* ActionLook = nullptr;

	//* Interact Input Action 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	UInputAction* ActionInteract = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	UInputAction* ActionItem = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	UInputAction* ActionItemLook = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	UInputAction* ActionMenu = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void InteractBegin(UCInteract* Comp);
	UFUNCTION()
	void InteractEnd(UCInteract* Comp);
	UFUNCTION()
	void MenuDone();
	UFUNCTION()
	void SetFB(float Value);
	
	//* Called for movement input 
	void ActMove(const FInputActionValue& Value);
	//* Called for looking input 
	void ActLook(const FInputActionValue& Value);
	void ActInteract();
	void ActItem();
	void ActItemLook();
	void ActMenu();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	//* Pawn mesh: 1st person view (arms; seen only by self) 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	// First person camera
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* Camera = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCInteractor* Interactor = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCLNoiser* Noiser = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettingsUI* SettingsUI = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UGameUI* UI = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
};

