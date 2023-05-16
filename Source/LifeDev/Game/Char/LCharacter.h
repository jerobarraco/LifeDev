// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "LifeDev/Game/Dialogs/DiagTypes.h"

#include "LCharacter.generated.h"

class UCInteractor;
class UCInteract;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
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
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UGameUI> UIClass = nullptr;

	//* MappingContext 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputMappingContext* Mapping = nullptr;

	//* Jump Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* InputJump;

	//* Move Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SetUp)
	UInputAction* InputMove;
		
	//* Look Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* InputLook;

	//* Interact Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SetUp)
	UInputAction* InputInteract;

protected:
	// UFUNCTION()
	// void InteractToggle(bool IsOn, UCInteract* Comp);
	UFUNCTION()
	void InteractStart(UCInteract* Comp);
	UFUNCTION()
	void InteractStop(UCInteract* Comp);

	UFUNCTION()
	void InteractPause(const FDialog& Diag);
	UFUNCTION()
	void InteractResume();
	
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//* Called for movement input 
	void ActMove(const FInputActionValue& Value);
	//* Called for looking input 
	void ActLook(const FInputActionValue& Value);
	void ActInteract(const FInputActionValue& Value);
	
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
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UGameUI* UI = nullptr;
};

