// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inventory/Inventory.h"

#include "LChar.generated.h"

class ULSettingsUI;
class UDialogs;
class UCInteractor;
class UCInteract;
class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGameUI;
class UCNoiser;

// base class for the character
UCLASS(config=Game)
class LIFEDEV_API ALChar : public ACharacter
{
	GENERATED_BODY()

public:
	ALChar();
	
	UFUNCTION(BlueprintCallable)
	void SetUIVisible(bool bCond);
	
	UFUNCTION(BlueprintCallable)
	void SetInputEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void InteractSetEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable)
	void LookItem(const FName& Name);

	UFUNCTION(BlueprintCallable)
	bool Say(const FName& Name);

	// factor to apply to look when hovering an interact
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float InteractDrag = .5;

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
	UFUNCTION()
	void InteractBegin(UCInteract* Comp);
	UFUNCTION()
	void InteractEnd(UCInteract* Comp);
	UFUNCTION()
	void MenuDone();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
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
	UPROPERTY(BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly)
	UCInteractor* Interactor = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UCNoiser* Noiser = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettingsUI* SettingsUI = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UGameUI* UI = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UDialogs* Dialogs = nullptr;
};

