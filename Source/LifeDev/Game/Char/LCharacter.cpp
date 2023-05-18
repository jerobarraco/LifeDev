// Copyright  Jerónimo Barraco-Mármol

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
#include "JUtils/MiscUtils.h"

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
	Interactor->SetEnabled(false);
}

void ALCharacter::InteractResume() {
	Interactor->SetEnabled(true);
}

void ALCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ToggleMapping(Mapping, 1, true, GetWorld());

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

	UWorld* const World = GetWorld();
	UDialogs* const UlDialogs = World->GetSubsystem<UDialogs>();
	UlDialogs->OnShow.AddUniqueDynamic(this, &ALCharacter::InteractPause);
	UlDialogs->OnHide.AddUniqueDynamic(this, &ALCharacter::InteractResume);
}

void ALCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;
	ToggleMapping(Mapping, 1, false, GetWorld());
	// TODO unbind actions
	Super::EndPlay(EndPlayReason);
}

/// Input

void ALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;
	//Jumping
	Input->BindAction(ActionJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(ActionJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//Moving
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &ALCharacter::ActMove);

	//Looking
	Input->BindAction(ActionLook, ETriggerEvent::Triggered, this, &ALCharacter::ActLook);
	Input->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &ALCharacter::ActInteract);
}


void ALCharacter::ActMove(const FInputActionValue& Value)
{
	// input is a Vector2D

	if (!Controller )return;

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
