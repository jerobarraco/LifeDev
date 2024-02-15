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
#include "LifeDev/Game/Interact/LInteract.h"

ALPuzzle::ALPuzzle():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CLPuzzle"));
	CPuzzle->DisableOnDone = true;

	Root->SetMobility(EComponentMobility::Static);

	#if WITH_EDITORONLY_DATA // inspired by AInfo
	UBillboardComponent* const SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (!IsRunningCommandlet() && (SpriteComponent != nullptr)) {
		// static ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture(TEXT("/Engine/EditorResources/S_Actor"));
		// static ConstructorHelpers::FObjectFinder<UTexture2D> CSprText(TEXT("/Engine/EditorResources/S_Actor"));
		static ConstructorHelpers::FObjectFinderOptional<UTexture2D>
			CSprTexture(TEXT("/Engine/EditorResources/S_Actor")); // S_Solver
		static const FName ID_Info = TEXT("Puzzle");
		static const FText ID_Name = NSLOCTEXT("SpriteCategory", "Puzzle", "Puzzle");

		SpriteComponent->SetupAttachment(Root);
		SpriteComponent->Sprite = CSprTexture.Get();
		// SpriteComponent->Sprite = CSprText.Object;
		SpriteComponent->SpriteInfo.Category = ID_Info;
		SpriteComponent->SpriteInfo.DisplayName = ID_Name;
		SpriteComponent->bIsScreenSizeScaled = true;
	}
	// setting spatially loaded to false could break datalayer usage which is critical
	#endif // WITH_EDITORONLY_DATA
}

void ALPuzzle::SetUseItemDlgs(const TMap<FName, FName>& Dlgs) {
	if (!CPuzzle) return;
	// set the dialogs on each registered interact
	TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	for (AInteract* const I: Inters) {
		ALInteract* const LI = Cast<ALInteract>(I);
		if (!IsValid(LI)) continue;
		// better to override the whole array than having issues down the line.
		// it also allows to remove stuff.
		LI->UseItemDlgs = Dlgs;	
	}
}

void ALPuzzle::SetStates(const TArray<int32>& States) {
	if (!CPuzzle) return;
	
	const TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = States.Num();
	const int32 Num2 = Inters.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Inters[i];
		if (!IsValid(I)) continue;
		I->SetState(States[i]);
	}

	// reload the ids if needed. important.
	CPuzzle->ResetCurrents();
}

void ALPuzzle::SetLocks(const TArray<bool>& Locks) {
	if (!CPuzzle) return;

	const TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	const int32 Num = Locks.Num();
	const int32 Num2 = Inters.Num();
	for (int32 i = 0; i<Num && i<Num2; ++i) {
		AInteract* const I = Inters[i];
		if (!IsValid(I)) continue;
		I->Locked = Locks[i];
	}
}

void ALPuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk) return;

	if (Diags) Diags->AddId(DoneDlg);
	if (FB) FB->ModVal(DoneFB);
	if (Flags) Flags->Mod(DoneFlag, 1);
	if (Inventory) Inventory->Mod(DoneItem, 1);
	if (Story && !DoneStep.IsNone()) Story->StartNext(DoneStep);

	if (IsValid(DoneInter)) {
		// force unlock
		DoneInter->Locked = false;
		DoneInter->TryTrigger();
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
	Inventory = UInventory::Instance(W);

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
	Inventory = nullptr;
	Story = nullptr;
}
