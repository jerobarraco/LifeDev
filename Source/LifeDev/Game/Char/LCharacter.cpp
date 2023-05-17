// Copyright  Jerónimo Barraco-Mármol

#include "LCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameUI.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Dialogs/Dialogs.h"
#include "Interact/CInteract.h"
#include "Interact/CInteractor.h"

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
	// TODO find a better option 
	// static ConstructorHelpers::FClassFinder<UInputMappingContext> DefaultMapping(TEXT("/Game/LifeDev/Core/Input/IMC_Default"));
	// Mapping = DefaultMapping.Class.GetDefaultObject();
	
	// static ConstructorHelpers::FClassFinder<UInputAction> CActionJump(TEXT("/Game/LifeDev/Core/Input/Actions/IA_Jump"));
	// ActionJump = CActionJump.Class.GetDefaultObject();
	// static ConstructorHelpers::FClassFinder<UInputAction> CActionLook(TEXT("/Game/LifeDev/Core/Input/Actions/IA_Look"));
	// ActionLook = CActionLook.Class.GetDefaultObject();
	// static ConstructorHelpers::FClassFinder<UInputAction> CActionMove(TEXT("/Game/LifeDev/Core/Input/Actions/IA_Move"));
	// ActionMove = CActionMove.Class.GetDefaultObject();

	// maybe this?
	// return Cast(StaticLoadObject( UInputMappingContext::StaticClass(), NULL, TEXT("/Game/Content/LifeDev/Core/Input/IMC_Default")));
}

void ALCharacter::SetUIVisible(bool Visible) {
	if (!IsValid(UI)) return;
	UI->SetVisibility(Visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

// void ALCharacter::InteractToggle(bool IsOn, UCInteract* Comp) {
// }

void ALCharacter::InteractStart(UCInteract* Comp) {
	if (IsValid(UI)) {
		UI->InteractShowPrompt(Comp->Text);
	}
}
void ALCharacter::InteractStop(UCInteract* Comp) {
	if (IsValid(UI)) {
		UI->InteractHidePrompt();
	}
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

	//Add Input Mapping Context
	APlayerController* const PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(Mapping, 0);
	}

	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UGameUI>(this, Class);
		UI->AddToViewport();
		// It is of utmost important that ANY canvas (and or root element) in the widget ui to be have its visibilty
		// set to HitTestInvisible or not hit testable. or it will "eat" the cursor
		// UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController); // doesn't do much. but neat to remember 
	}
	// Interactor->OnToggle.AddUniqueDynamic(this, &ALCharacter::InteractToggle);
	Interactor->OnStart.AddUniqueDynamic(this, &ALCharacter::InteractStart);
	Interactor->OnStop.AddUniqueDynamic(this, &ALCharacter::InteractStop);

	UWorld* const World = GetWorld();
	UDialogs* const UlDialogs = World->GetSubsystem<UDialogs>();
	UlDialogs->OnShow.AddUniqueDynamic(this, &ALCharacter::InteractPause);
	UlDialogs->OnStop.AddUniqueDynamic(this, &ALCharacter::InteractResume);
}

void ALCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(UI)) {
		UI->RemoveFromParent();
	}
	UI = nullptr;

	Super::EndPlay(EndPlayReason);
}

/// Input

void ALCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input) return;
	//Jumping
	Input->BindAction(InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(InputJump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//Moving
	Input->BindAction(InputMove, ETriggerEvent::Triggered, this, &ALCharacter::ActMove);

	//Looking
	Input->BindAction(InputLook, ETriggerEvent::Triggered, this, &ALCharacter::ActLook);
	Input->BindAction(InputInteract, ETriggerEvent::Triggered, this, &ALCharacter::ActInteract);
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
