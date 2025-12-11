// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "LChar.generated.h"

enum class EFeat : uint8;
class UCLCharArm;
class UCLDust;
class UFlags;
class UInventory;
class UCLCharCam;
class UCLCharItems;
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

// TODO consider moving the input stuff to another component (some to the player controller)
// TODO consider moving the settings to another component

// Can't move to the movement component as the class is hardcoded to UMovementComponent.

// base class for the character
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALChar : public ACharacter {
	GENERATED_BODY()

public:
#pragma region base
	ALChar();

	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ALChar* Instance(const UObject* const O);

	// Called for the internal init
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, ForceAsFunction))
	void Init();
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay, ForceAsFunction))
	void DeInit();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void SetUIVisible(const bool Visible);

	// fully disables the input of the character. use InteractSetEnabled instead unless you really need to.
	UFUNCTION(BlueprintCallable)
	void SetInputEnabled(const bool Enabled);

	// disable the interact for the character.
	UFUNCTION(BlueprintCallable)
	void InteractSetActive(const bool Enabled) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCInteractor* GetInteractor() const {return Interactor; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCLCharItems* GetCharItems() const { return Items; }

	// .3 is too little and annoying.
	static constexpr float DefInteractDrag = .5;
	// factor to apply to look when hovering an Interact.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float InteractDrag = DefInteractDrag;

	// TODO fix speedMin and Max on config.
	// Somehow unreal saves the values after a play.
	// since foxy changes it. it saves a modified value. that later, gets foxified again.
	// so it will drift on each playthrough. The possible solution is not to modify these vars.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float SpeedMin = 65;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float SpeedMax = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=SetUp)
	float SpeedFoxy = 10;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 InputPrio = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UGameUI> UIClass = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UInputMappingContext> CtxChar = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UInputMappingContext> CtxItems = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionJump = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TObjectPtr<UInputAction> ActionMove = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionLook = nullptr;

	//* Interact Input Action 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionInteract = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionItemUse = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = SetUp)
	TObjectPtr<UInputAction> ActionItemLook = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* const NewController) override;

	UFUNCTION()
	void InteractHover(const bool On, UCInteract* const Comp);
	UFUNCTION()
	void SetFB(const float Value);
	UFUNCTION()
	void FeatUp(const EFeat Feat, const bool Enabled);
	
	void ActMove(const FInputActionValue& Value); // movement input
	void ActLook(const FInputActionValue& Value); // camera look aim
	void ActInteract();
	void ActItem();
	void ActItemLook();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* const InputComponent) override;
	// End of APawn interface

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=Camera)
	float ViewPitchMax = 70;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Config, Category=Mesh)
	float HoverDiagTime = 2;

	// First person camera
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Camera)
	TObjectPtr<UCLCharCam> Camera = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Camera)
	TObjectPtr<UCLCharArm> Arm = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCInteractor> Interactor = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCLCharItems> Items = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCLNoiser> Noiser = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCLDust> Dust = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UGameUI> UI = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
};

