// Copyright Jerónimo Barraco-Mármol

#include "LPuzzle.h"

#include "Components/BillboardComponent.h"
#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Interact/Interact.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALPuzzle::ALPuzzle():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CLPuzzle"));
	
	Root->SetMobility(EComponentMobility::Static);

	#if WITH_EDITORONLY_DATA // inspired by AInfo
	UBillboardComponent* const SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!IsRunningCommandlet() && (SpriteComponent != nullptr)) {
		// static ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture(TEXT("/Engine/EditorResources/S_Actor"));
		// static ConstructorHelpers::FObjectFinder<UTexture2D> CSprText(TEXT("/Engine/EditorResources/S_Actor"));
		static ConstructorHelpers::FObjectFinderOptional<UTexture2D>
			CSprTexture(TEXT("/Engine/EditorResources/S_Solver"));
		static const FName ID_Info = TEXT("Info");
		static const FText NAME_Info = NSLOCTEXT("SpriteCategory", "Info", "Info");

		SpriteComponent->SetupAttachment(Root);
		SpriteComponent->Sprite = CSprTexture.Get();
		// SpriteComponent->Sprite = CSprText.Object;
		SpriteComponent->SpriteInfo.Category = ID_Info;
		SpriteComponent->SpriteInfo.DisplayName = NAME_Info;
		SpriteComponent->bIsScreenSizeScaled = true;
	}
	// setting spatially loaded to false could break datalayer usage which is critical
	#endif // WITH_EDITORONLY_DATA

}

void ALPuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk) return;

	if (Diags) Diags->AddId(DoneDlg);
	if (FB) FB->ModVal(DoneFB);
	if (Flags) Flags->Mod(DoneFlag, 1);
	if (Items) Items->Mod(DoneItem, 1);
	if (!DoneStep.IsNone() && Story) Story->StartNext(DoneStep);

	if (IsValid(Interact)) {
		// force unlock
		Interact->Locked = false;
		Interact->TryTrigger();
	}
}

void ALPuzzle::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	if (!W) return;
	
	Story = UStory::Instance(W);
	FB = UFlashback::Instance(W);
	Flags = UFlags::Instance(W);
	Diags = UDiags::Instance(W);
	Items = UInventory::Instance(W);

	CPuzzle->OnDone.AddUniqueDynamic(this, &ALPuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &ALPuzzle::Update);
}

void ALPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (CPuzzle) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
	}

	Flags = nullptr;
	FB = nullptr;
	Diags = nullptr;
	Items = nullptr;
	Story = nullptr;
}
