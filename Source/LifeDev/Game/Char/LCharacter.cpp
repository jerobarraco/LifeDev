// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h" // for the get object
#include "GameUI.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Dialogs/Dialogs.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "JUtils/JMiscUtils.h"

// ALifeDevCharacter

ALCharacter::ALCharacter(): Super()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	Camera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Interactor = CreateDefaultSubobject<UCInteractor>(TEXT("Interactor"));
	Interactor->SetupAttachment(Camera);
	
	UIClass = UGameUI::StaticClass();
	
	SetActorTickEnabled(false);
	// load the ui class here with the class finder.
	// and also all the other default objects
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultUI(TEXT("/Game/LifeDev/Game/Char/W_GameUI"));
	UIClass = DefaultUI.Class;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionJump(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Jump"));
	ActionJump = CActionJump.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionLook(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Look"));
	ActionLook = CActionLook.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionMove(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Move"));
	ActionMove = CActionMove.Object;
	// This version does NOT work
	// static ConstructorHelpers::FClassFinder<UInputAction> CActionInteract(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Interact"));
	// ActionInteract = CActionInteract.Class.GetDefaultObject();
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionInteract(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Interact"));
	ActionInteract = CActionInteract.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionItem(TEXT("/Game/LifeDev/Game/Char/Input/Actions/IA_Item"));
	ActionItem = CActionItem.Object;
}

void ALCharacter::SetUIVisible(bool Visible) {
	if (!IsValid(UI)) return;
	UI->SetVisibility(Visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

// void ALCharacter::InteractToggle(bool IsOn, UCInteract* Comp) {
// }

void ALCharacter::InteractBegin(UCInteract* Comp) {
	if (!IsValid(UI)) return;
	UI->InteractShowPrompt(Comp->Text);
}

void ALCharacter::InteractEnd(UCInteract* Comp) {
	if (!IsValid(UI)) return;
	UI->InteractHidePrompt();
}

void ALCharacter::InteractPause(const FDialog& Diag) {
	SetInputEnabled(false);
}

void ALCharacter::InteractResume() {
	// TODO only re-enable if it was enabled before pause. Also rename to DialogShows or smth
	SetInputEnabled(true);
}

void ALCharacter::SetInputEnabled(bool Enabled) {
	UI->SetVisibility(Enabled? ESlateVisibility::Visible: ESlateVisibility::Hidden);
	Interactor->SetEnabled(Enabled);
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, Enabled, GetWorld());
}

void ALCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, true, World);
	
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UGameUI>(this, Class);
		UI->AddToViewport();
		UI->InteractHidePrompt();
		// It is of utmost important that ANY canvas (and or root element) in the widget ui to be have its visibilty
		// set to HitTestInvisible or not hit testable. or it will "eat" the cursor
		// UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController); // doesn't do much. but neat to remember 
	}
	// Interactor->OnToggle.AddUniqueDynamic(this, &ALCharacter::InteractToggle);
	Interactor->OnBegin.AddUniqueDynamic(this, &ALCharacter::InteractBegin);
	Interactor->OnEnd.AddUniqueDynamic(this, &ALCharacter::InteractEnd);

	UDialogs* const UlDialogs = World->GetSubsystem<UDialogs>();
	UlDialogs->OnShow.AddUniqueDynamic(this, &ALCharacter::InteractPause);
	UlDialogs->OnDone.AddUniqueDynamic(this, &ALCharacter::InteractResume);

	Inventory = World->GetSubsystem<UInventory>();
}

void ALCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;
	Inventory = nullptr;

	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
	// TODO unbind actions
	Super::EndPlay(EndPlayReason);
}

/// Input

void ALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;

	Input->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(ActionJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ALCharacter::ActMove);
	Input->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ALCharacter::ActLook);
	Input->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &ALCharacter::ActInteract);
	Input->BindAction(ActionItem, ETriggerEvent::Triggered, this, &ALCharacter::ActItem);
}


void ALCharacter::ActMove(const FInputActionValue& Value)
{
	// input is a Vector2D

	if (!Controller) return;

	const FVector2D& MovementVector = Value.Get<FVector2D>();
	// add movement 
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ALCharacter::ActLook(const FInputActionValue& Value)
{
	// input is a Vector2D

	if (!Controller) return;
	const FVector2D& LookAxisVector = Value.Get<FVector2D>();
	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ALCharacter::ActInteract(const FInputActionValue& Value) {
	if (!Controller) return;
	if (!Interactor) return;
	Interactor->TryTrigger();
}

void ALCharacter::ActItem() {
	if (!IsValid(Inventory)) return;
	const FName& Selected = Inventory->GetSelected();
	UE_LOG(LogTemp, Log, TEXT("ActItem=%s"), *Selected.ToString());

	if (Selected.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("No item is selected."));
		return;
	}

	if (!Inventory->IsCold(Selected)) {
		UE_LOG(LogTemp, Warning, TEXT("Item is not cold."));
		return;
	}

	// TODO this needs to be improved
	FItem Item;
	if (!Inventory->Get(Selected, Item)) {
		UE_LOG(LogTemp, Warning, TEXT("Item does not exists? but here? this should NOT happen!!!!"));
		return;
	}

	if (!Item.Usable) {
		// TODO open the ui and show it
		UE_LOG(LogTemp, Warning, TEXT(
			"This is a erzats display for the item '%s'. Look how beautiful it is!"
			"you have %i of them, and the description is : '%s'. "),
			*Item.Title.ToString(), Item.Count, *Item.Description.ToString())
		return;
	}

	// TODO this try use item and inventory.use are out of sync. either of them could fail...
	// TODO find a better way
	if (!Interactor->TryUseItem(Selected)){
		// notice only checking auto-trigger here. so that i can use an auto trigger with an interact too
		if (Item.AutoTrigger) {
			// TODO trigger effect here
			UE_LOG(LogTemp, Warning, TEXT("Stub effect trigger for item '%s'."), *Item.Title.ToString());
		} else {
			UE_LOG(LogTemp, Log, TEXT("Can't use item with that."));
			return;
		}
	}

	Inventory->Use(Selected);
}
